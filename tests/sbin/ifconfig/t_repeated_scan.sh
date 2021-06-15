# $NetBSD: t_repeated_scan.sh,v 1.6 2020/12/10 08:16:59 mrg Exp $
#
# Copyright (c) 2020 The NetBSD Foundation, Inc.
# All rights reserved.
#
# This code is derived from software contributed to The NetBSD Foundation
# by Jukka Ruohonen.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
# ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

atf_test_case repeated_scan
repeated_scan_head() {
	atf_set "require.user" "root"
	atf_set "descr" "Test with ifconfig(8) that repeated 802.11 " \
		"scanning does not panic (PR kern/53860; PR kern/55389)"
}

repeated_scan_body() {

	if ! [ $(atf_config_get "run_unsafe" "no") = "yes" ]
	then
		atf_skip "can disrupt networking; also PR port-evbarm/55521"
	fi

	# Even though this should amount to a "few minutes",
	# try to still avoid stalling any automated test runs.
	#
	n=15

	pkill -9 hostapd
	pkill -9 wpa_supplicant

	for i in $(ifconfig -l); do

		state="up"
		ifconfig -s $i

		if [ $? -eq 1 ]; then

			state="down"
			ifconfig $i up

			if [ ! $? -eq 0 ]; then
				echo "Failed to set $i up"
				continue
			fi
		fi

		sleep 1
		ifconfig $i list scan >/dev/null 2>&1

		if [ ! $? -eq 0 ]; then
			echo "Skipping $i; scan not supported"
			continue
		fi

		while [ $n -gt 0 ]; do
			echo "Test $n for $i"
			ifconfig $i list scan >/dev/null 2>&1
			n=$(expr $n - 1)
		done

		ifconfig $i $state
		echo "Restored state of $i to $state"
		sleep 1
	done

	/bin/sh /etc/rc.d/hostapd restart >/dev/null 2>&1
	/bin/sh /etc/rc.d/wpa_supplicant restart >/dev/null 2>&1

	atf_pass
}

atf_init_test_cases() {
	atf_add_test_case repeated_scan
}
