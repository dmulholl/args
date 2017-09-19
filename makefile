# --------------------------------------------------------------------------
# Make variables.
# --------------------------------------------------------------------------

CFLAGS = -Wall -Wextra --std=c99 --pedantic

# --------------------------------------------------------------------------
# Targets.
# --------------------------------------------------------------------------

example: src/example.c src/cops.c src/cops.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/example src/example.c src/cops.c

unittests: src/tests.c src/cops.c src/cops.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -Wno-unused-parameter -o bin/unittests src/tests.c src/cops.c

check:
	@make unittests
	./bin/unittests

clean:
	rm -f ./src/*.o
	rm -f ./bin/unittests
	rm -f ./bin/example
