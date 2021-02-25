# ------------------------------------------------------------------------------
# Make variables.
# ------------------------------------------------------------------------------

CFLAGS = -Wall -Wextra --std=c99 --pedantic -Wno-unused-parameter

# ------------------------------------------------------------------------------
# Phony targets.
# ------------------------------------------------------------------------------

all::
	@make lib
	@make ex1
	@make ex2
	@make tests

lib::
	@mkdir -p bin
	$(CC) $(CFLAGS) -c -o bin/args.o src/args.c

ex1::
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/ex1 src/example1.c src/args.c

ex2::
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/ex2 src/example2.c src/args.c

tests::
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/tests src/tests.c src/args.c

check::
	@make tests
	./bin/tests

clean::
	rm -f ./bin/*
