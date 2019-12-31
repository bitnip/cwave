SOURCE= \
	src/wave.c \
	src/wave_parser.c
TEST_SOURCE= \
	src/test.c \
	src/wave_parser_test.c
LIBRARIES=-L../cutil/bin -lcutil
INCLUDES=-I../