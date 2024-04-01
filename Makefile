TARGET := $(shell $(CC) -dumpmachine)
WINDOWS := $(strip $(foreach W,msys w64 windows,$(findstring -$W-,-$(TARGET)-)))
BINEXT := $(if $(WINDOWS),.exe,)

ERROR_CFLAGS := -Wall -Wc++-compat -Wconversion -Werror -Wextra -Wpedantic -Wvla
ERROR_CFLAGS := $(ERROR_CFLAGS) -Wno-strict-prototypes
CFLAGS := $(CFLAGS) $(ERROR_CFLAGS)

HUFFMAN_CFLAGS := $(CFLAGS) -Iinclude/
HUFFMAN_MAIN_SRC := src/main.c src/testes.c
HUFFMAN_SRC := include/*.h $(filter-out $(HUFFMAN_MAIN_SRC),$(wildcard src/*.c))

TEST_CFLAGS := $(HUFFMAN_CFLAGS) $(shell pkgconf --cflags cunit)
TEST_LDLIBS := $(shell pkgconf --libs cunit)

.PHONY: build docs

build: build/huffman$(BINEXT) build/plotagem$(BINEXT) build/testes$(BINEXT)

build/:
	mkdir $@

build/huffman$(BINEXT): $(HUFFMAN_SRC) src/main.c | build/
	$(CC) $(HUFFMAN_CFLAGS) -o $@ $(filter %.c,$^)

build/plotagem$(BINEXT): plotagem/plotagem.c
	$(CC) $(CFLAGS) -o $@ $(filter %.c,$^)

build/testes$(BINEXT): $(HUFFMAN_SRC) src/testes.c | build/
	$(CC) $(TEST_CFLAGS) -o $@ $(filter %.c,$^) $(TEST_LDLIBS)

docs:
	doxygen
