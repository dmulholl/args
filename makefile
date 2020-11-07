# ------------------------------------------------------------------------------
# Variables.
# ------------------------------------------------------------------------------

CFLAGS = -Wall -Wextra --std=c99 --pedantic -Wno-unused-parameter

# ------------------------------------------------------------------------------
# Targets.
# ------------------------------------------------------------------------------

examples: src/example1.c src/example2.c src/args.c src/args.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/example1 src/example1.c src/args.c
	$(CC) $(CFLAGS) -o bin/example2 src/example2.c src/args.c

unittests: src/tests.c src/args.c src/args.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/tests src/tests.c src/args.c

check:
	@make unittests
	./bin/tests

clean:
	rm -f ./src/*.o
	rm -f ./bin/tests
	rm -f ./bin/example1
	rm -f ./bin/example2
