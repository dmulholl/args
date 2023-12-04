# ----------- #
#  Variables  #
# ----------- #

CFLAGS = -Wall -Wextra --std=c99 --pedantic -Wno-unused-parameter

# --------------- #
#  Phony Targets  #
# --------------- #

help: ## Prints available commands.
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z0-9_-]+:.*?## / \
	{printf "\033[1;36m%-20s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)

all: ## Compiles all binaries.
	@make ex1
	@make ex2
	@make tests

ex1: ## Compiles the example1.c file.
	@mkdir -p build
	$(CC) $(CFLAGS) -o build/ex1 src/example1.c src/args.c

ex2: ## Compile. the example2.c file.
	@mkdir -p build
	$(CC) $(CFLAGS) -o build/ex2 src/example2.c src/args.c

tests: ## Compiles the test binary.
	@mkdir -p build
	$(CC) $(CFLAGS) -o build/tests src/tests.c src/args.c

check: ## Runs tests.
	@make tests
	./build/tests

clean: ## Deletes all build artifacts.
	rm -f ./build/*
