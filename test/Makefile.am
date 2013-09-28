AUTOMAKE_OPTIONS = serial-tests 
ACLOCAL_AMFLAGS = -I autotools
AM_CXXFLAGS =  -DGTEST_HAS_PTHREAD=0 -I$(top_srcdir)/test/ -I$(top_srcdir)/include/
check_PROGRAMS = gtest_all_test
TESTS_ENVIRONMENT = GTEST_OUTPUT="@GTEST_OUTPUT@"; export GTEST_OUTPUT;
TESTS = gtest_all_test
gtest_all_test_SOURCES = gtest_all_test.cpp gtest-all.cc gtest_main.cc
gtest_all_test_LDADD = 
noinst_HEADERS = gtest.h
clean-local:
	-rm -rf results
