SOURCE= \
	src/wave.c \
	src/wave_parser.c
TEST_SOURCE= \
	src/test.c \
	src/wave_parser_test.c
LIBRARIES=-L../cutil/bin -lcutil
INCLUDES=-I../

COVERAGE_CC=gcc
ifeq ($(shell uname -s),Darwin)
	CC=gcc
endif
ifeq ($(shell uname -s),Linux)
	CC=gcc
endif
ifeq ($(OS),Windows_NT)
	CC=x86_64-w64-mingw32-gcc
endif