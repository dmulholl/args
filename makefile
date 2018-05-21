# ------------------------------------------------------------------------------
# Make variables.
# ------------------------------------------------------------------------------

CFLAGS = -Wall -Wextra --std=c99 --pedantic

# ------------------------------------------------------------------------------
# Targets.
# ------------------------------------------------------------------------------

example: src/example.c src/janus.c src/janus.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/example src/example.c src/janus.c

unittests: src/tests.c src/janus.c src/janus.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -Wno-unused-parameter -o bin/unittests src/tests.c src/janus.c

check:
	@make unittests
	./bin/unittests

clean:
	rm -f ./src/*.o
	rm -f ./bin/unittests
	rm -f ./bin/example
