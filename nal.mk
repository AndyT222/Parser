CFLAGS = -g3 -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
DFLAGS = -g3 -fsanitize=undefined -fsanitize=address
CC = gcc
OUTP = interp parse
SOURCE1 = parse.c nal.c nal.h
SOURCE2 = interp.c nal.c nal.h

# There are two test versions - explained in testing.txt#
TEST = testinterp testinterp.c nal.c
DEBUG = debug-interp debug-interp.c deb-nal.c

TESTP = testparse testparse.c nal.c
DEBUGP = debug-parse debug-parse.c deb-nal.c

# Default is to compile both with no test flags #
all: parse.c interp.c nal.c nal.h 
	$(CC) -o parse $(SOURCE1) && $(CC) -o interp $(SOURCE2)

# Compile parse/interp individually #
parse:
	$(CC) -o parse $(SOURCE1)

interp:
	$(CC) -o interp $(SOURCE2)

# Compile both with compiler flags #
testflags:
	$(CC) $(CFLAGS) -o parse_t $(SOURCE1) && $(CC) $(CFLAGS) -o interp_t $(SOURCE2)

memleaks:
	$(CC) $(DFLAGS) -o parse_m $(SOURCE1) && $(CC) $(DFLAGS) -o interp_m $(SOURCE2)

# Debug & test versions #
testparse:
	$(CC) $(CFLAGS) -o $(TESTP) && $(CC) $(CFLAGS) -o $(DEBUGP)

testinterp:
	$(CC) $(CFLAGS) -o $(TEST) && $(CC) $(CFLAGS) -o $(DEBUG)

clean:
	rm -f $(OUTP)

