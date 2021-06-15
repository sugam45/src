# Copyright (c) 2018 The NetBSD Foundation, Inc.
# All rights reserved.
#
# This code is derived from software contributed to The NetBSD Foundation
# by Yang Zheng.
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
#

tsan_available_archs()
{
	atf_set "require.arch" "x86_64"
}

test_target()
{
	SUPPORT='n'
	# Detect address space larger than 32 bits
	maxaddress=`sysctl vm.maxaddress|awk '{print $3}'`
	if [ $maxaddress -gt 4294967295 ]; then
		if command -v cc >/dev/null 2>&1; then
			if ! echo __clang__ | cc -E - | grep -q __clang__; then
				SUPPORT='y'
			elif ! cc -v 2>&1 | awk '/gcc version/{print $3}' | \
				awk -F '.' '($0+0) > 9 {exit 1}'; then
				SUPPORT='y'
			fi
		fi
	fi
}

atf_test_case heap_use_after_free
heap_use_after_free_head() {
	atf_set "descr" "Test thread sanitizer for use-after-free condition"
	atf_set "require.progs" "c++ paxctl"
	tsan_available_archs
}

atf_test_case heap_use_after_free_profile
heap_use_after_free_profile_head() {
	atf_set "descr" "Test thread sanitizer for use-after-free with profiling option"
	atf_set "require.progs" "c++ paxctl"
	tsan_available_archs
}
atf_test_case heap_use_after_free_pic
heap_use_after_free_pic_head() {
	atf_set "descr" "Test thread sanitizer for use-after-free with position independent code (PIC) flag"
	atf_set "require.progs" "c++ paxctl"
	tsan_available_archs
}
atf_test_case heap_use_after_free_pie
heap_use_after_free_pie_head() {
	atf_set "descr" "Test thread sanitizer for use-after-free with position independent execution (PIE) flag"
	atf_set "require.progs" "c++ paxctl"
	tsan_available_archs
}

heap_use_after_free_body(){
	cat > test.cc << EOF
#include <pthread.h>
#include <stdlib.h>

int *ptr;
pthread_barrier_t barrier;
void *Thread(void *a) {
  pthread_barrier_wait(&barrier);
  *ptr = 42;
  return 0;
}

int main() {
  pthread_t t;
  pthread_barrier_init(&barrier, NULL, 2);
  ptr = (int *)malloc(sizeof(int));
  pthread_create(&t, NULL, Thread, NULL);
  free(ptr);
  pthread_barrier_wait(&barrier);
  pthread_join(t, NULL);
  return 0;
}
EOF

	c++ -fsanitize=thread -o test test.cc
	paxctl +a test
	atf_check -s ignore -o ignore -e match:"WARNING: ThreadSanitizer: heap-use-after-free" ./test
}

heap_use_after_free_profile_body(){
	atf_expect_fail "PR toolchain/55760"
	cat > test.cc << EOF
#include <pthread.h>
#include <stdlib.h>

int *ptr;
pthread_barrier_t barrier;
void *Thread(void *a) {
  pthread_barrier_wait(&barrier);
  *ptr = 42;
  return 0;
}

int main() {
  pthread_t t;
  pthread_barrier_init(&barrier, NULL, 2);
  ptr = (int *)malloc(sizeof(int));
  pthread_create(&t, NULL, Thread, NULL);
  free(ptr);
  pthread_barrier_wait(&barrier);
  pthread_join(t, NULL);
  return 0;
}
EOF

	c++ -fsanitize=thread -o test -pg test.cc
	paxctl +a test
	atf_check -s ignore -o ignore -e match:"WARNING: ThreadSanitizer: heap-use-after-free" ./test
}

heap_use_after_free_pic_body(){
	cat > test.cc << EOF
#include <stdio.h>
#include <stdlib.h>
int help(int);
int main(int argc, char **argv) {return help(argc);}
EOF

	cat > pic.cc << EOF
#include <pthread.h>
#include <stdlib.h>

int *ptr;
pthread_barrier_t barrier;
void *Thread(void *a) {
  pthread_barrier_wait(&barrier);
  *ptr = 42;
  return 0;
}

int help(int argc) {
  pthread_t t;
  pthread_barrier_init(&barrier, NULL, 2);
  ptr = (int *)malloc(sizeof(int));
  pthread_create(&t, NULL, Thread, NULL);
  free(ptr);
  pthread_barrier_wait(&barrier);
  pthread_join(t, NULL);
  return 0;
}
EOF

	c++ -fsanitize=thread -fPIC -shared -o libtest.so pic.cc
	c++ -o test test.cc -fsanitize=thread -L. -ltest
	paxctl +a test

	export LD_LIBRARY_PATH=.
	atf_check -s ignore -o ignore -e match:"WARNING: ThreadSanitizer: heap-use-after-free" ./test
}
heap_use_after_free_pie_body(){
	
	#check whether -pie flag is supported on this architecture
	if ! c++ -pie -dM -E - < /dev/null 2>/dev/null >/dev/null; then 
		atf_set_skip "c++ -pie not supported on this architecture"
	fi
	cat > test.cc << EOF
#include <pthread.h>
#include <stdlib.h>

int *ptr;
pthread_barrier_t barrier;
void *Thread(void *a) {
  pthread_barrier_wait(&barrier);
  *ptr = 42;
  return 0;
}

int main() {
  pthread_t t;
  pthread_barrier_init(&barrier, NULL, 2);
  ptr = (int *)malloc(sizeof(int));
  pthread_create(&t, NULL, Thread, NULL);
  free(ptr);
  pthread_barrier_wait(&barrier);
  pthread_join(t, NULL);
  return 0;
}
EOF

	c++ -fsanitize=thread -o test -fpie -pie test.cc
	paxctl +a test
	atf_check -s ignore -o ignore -e match:"WARNING: ThreadSanitizer: heap-use-after-free" ./test
}


atf_init_test_cases()
{
	atf_add_test_case heap_use_after_free
	atf_add_test_case heap_use_after_free_profile
	atf_add_test_case heap_use_after_free_pie
	atf_add_test_case heap_use_after_free_pic
}
