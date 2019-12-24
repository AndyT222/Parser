CFLAGS = -g3 -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
DFLAGS = -g3 -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
SFLAGS = -g3 -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi

TESTBASE = interp
INCS = nal.h
SOURCES =  $(TESTBASE).c nal.c

EXECS = $(TESTBASE) $(TESTBASE)_d $(TESTBASE)_s interp
CC = gcc

run: $(TESTBASE)
	./$(TESTBASE) 

all: $(EXECS)

$(TESTBASE): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

$(TESTBASE)_d: $(SOURCES) $(INCS) 
	$(CC) $(SOURCES) -o $@ $(DFLAGS) 

$(TESTBASE)_s: $(SOURCES) $(INCS) 
	$(CC) $(SOURCES) -o $@ $(SFLAGS) 

clean:
	rm -f $(EXECS)

memleaks: $(TESTBASE)_d $(TESTBASE)_s
	@echo "Sanitize :"
	@./$(TESTBASE)_s
	@echo "Valgrind :"
	@valgrind --leak-check=full ./$(TESTBASE)_d
