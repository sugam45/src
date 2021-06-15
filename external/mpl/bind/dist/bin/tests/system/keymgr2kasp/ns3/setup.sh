#!/bin/sh -e
#
# Copyright (C) Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, you can obtain one at https://mozilla.org/MPL/2.0/.
#
# See the COPYRIGHT file distributed with this work for additional
# information regarding copyright ownership.

# shellcheck source=conf.sh
. ../../conf.sh

echo_i "ns3/setup.sh"

setup() {
	zone="$1"
	echo_i "setting up zone: $zone"
	zonefile="${zone}.db"
	infile="${zone}.db.infile"
}

private_type_record() {
	_zone=$1
	_algorithm=$2
	_keyfile=$3

	_id=$(keyfile_to_key_id "$_keyfile")

	printf "%s. 0 IN TYPE65534 %s 5 %02x%04x0000\n" "$_zone" "\\#" "$_algorithm" "$_id"
}


# Make lines shorter by storing key states in environment variables.
H="HIDDEN"
R="RUMOURED"
O="OMNIPRESENT"
U="UNRETENTIVE"

# Set up a zone with auto-dnssec maintain to migrate to dnssec-policy.
setup migrate.kasp
echo "$zone" >> zones
ksktimes="-P now -A now -P sync now"
zsktimes="-P now -A now"
KSK=$($KEYGEN -a $DEFAULT_ALGORITHM -L 7200 -f KSK $ksktimes $zone 2> keygen.out.$zone.1)
ZSK=$($KEYGEN -a $DEFAULT_ALGORITHM -L 7200        $zsktimes $zone 2> keygen.out.$zone.2)
cat template.db.in "${KSK}.key" "${ZSK}.key" > "$infile"
private_type_record $zone $DEFAULT_ALGORITHM_NUMBER "$KSK" >> "$infile"
private_type_record $zone $DEFAULT_ALGORITHM_NUMBER "$ZSK" >> "$infile"
$SIGNER -S -x -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Set up a zone with auto-dnssec maintain to migrate to dnssec-policy, but this
# time the existing keys do not match the policy.  The existing keys are
# RSASHA1 keys, and will be migrated to a dnssec-policy that dictates
# ECDSAP256SHA256 keys.
setup migrate-nomatch-algnum.kasp
echo "$zone" >> zones
Tds="now-3h"     # Time according to dnssec-policy that DS will be OMNIPRESENT
Tkey="now-3900s" # DNSKEY TTL + propagation delay
Tsig="now-12h"   # Zone's maximum TTL + propagation delay
ksktimes="-P ${Tkey} -A ${Tkey} -P sync ${Tds}"
zsktimes="-P ${Tkey} -A ${Tsig}"
KSK=$($KEYGEN -a RSASHA1 -b 2048 -L 300 -f KSK $ksktimes $zone 2> keygen.out.$zone.1)
ZSK=$($KEYGEN -a RSASHA1 -b 1024 -L 300        $zsktimes $zone 2> keygen.out.$zone.2)
cat template.db.in "${KSK}.key" "${ZSK}.key" > "$infile"
private_type_record $zone 5 "$KSK" >> "$infile"
private_type_record $zone 5 "$ZSK" >> "$infile"
$SIGNER -S -x -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Set up a zone with auto-dnssec maintain to migrate to dnssec-policy, but this
# time the existing keys do not match the policy.  The existing keys are
# 1024 bits RSASHA1 keys, and will be migrated to a dnssec-policy that
# dictates 2048 bits RSASHA1 keys.
setup migrate-nomatch-alglen.kasp
echo "$zone" >> zones
Tds="now-3h"     # Time according to dnssec-policy that DS will be OMNIPRESENT
Tkey="now-3900s" # DNSKEY TTL + propagation delay
Tsig="now-12h"   # Zone's maximum TTL + propagation delay
ksktimes="-P ${Tkey} -A ${Tkey} -P sync ${Tds}"
zsktimes="-P ${Tkey} -A ${Tsig}"
KSK=$($KEYGEN -a RSASHA1 -b 1024 -L 300 -f KSK $ksktimes $zone 2> keygen.out.$zone.1)
ZSK=$($KEYGEN -a RSASHA1 -b 1024 -L 300        $zsktimes $zone 2> keygen.out.$zone.2)
cat template.db.in "${KSK}.key" "${ZSK}.key" > "$infile"
private_type_record $zone 5 "$KSK" >> "$infile"
private_type_record $zone 5 "$ZSK" >> "$infile"
$SIGNER -S -x -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

#
# Set up zones to test time metadata correctly sets state.
#

# Key states expected to be rumoured after migration.
setup rumoured.kasp
echo "$zone" >> zones
Tds="now-2h"
Tkey="now-300s"
Tsig="now-11h"
ksktimes="-P ${Tkey} -A ${Tkey} -P sync ${Tds}"
zsktimes="-P ${Tkey} -A ${Tsig}"
KSK=$($KEYGEN -a $DEFAULT_ALGORITHM -L 300 -f KSK $ksktimes $zone 2> keygen.out.$zone.1)
ZSK=$($KEYGEN -a $DEFAULT_ALGORITHM -L 300        $zsktimes $zone 2> keygen.out.$zone.2)
cat template.db.in "${KSK}.key" "${ZSK}.key" > "$infile"
private_type_record $zone $DEFAULT_ALGORITHM_NUMBER "$KSK" >> "$infile"
private_type_record $zone $DEFAULT_ALGORITHM_NUMBER "$ZSK" >> "$infile"
$SIGNER -S -x -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Key states expected to be omnipresent after migration.
setup omnipresent.kasp
echo "$zone" >> zones
Tds="now-3h"     # Time according to dnssec-policy that DS will be OMNIPRESENT
Tkey="now-3900s" # DNSKEY TTL + propagation delay
Tsig="now-12h"   # Zone's maximum TTL + propagation delay
ksktimes="-P ${Tkey} -A ${Tkey} -P sync ${Tds}"
zsktimes="-P ${Tkey} -A ${Tsig}"
KSK=$($KEYGEN -a $DEFAULT_ALGORITHM -L 300 -f KSK $ksktimes $zone 2> keygen.out.$zone.1)
ZSK=$($KEYGEN -a $DEFAULT_ALGORITHM -L 300        $zsktimes $zone 2> keygen.out.$zone.2)
cat template.db.in "${KSK}.key" "${ZSK}.key" > "$infile"
private_type_record $zone $DEFAULT_ALGORITHM_NUMBER "$KSK" >> "$infile"
private_type_record $zone $DEFAULT_ALGORITHM_NUMBER "$ZSK" >> "$infile"
$SIGNER -S -x -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1
