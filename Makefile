TARGET := $(shell $(CC) -dumpmachine)
WINDOWS := $(strip $(foreach W,msys w64 windows,$(findstring -$W-,-$(TARGET)-)))
BINEXT := $(if $(WINDOWS),.exe,)

ERROR_CFLAGS := -Wall -Wc++-compat -Wconversion -Werror -Wextra -Wpedantic -Wvla
CFLAGS := $(CFLAGS) $(ERROR_CFLAGS)

HUFFMAN_CFLAGS := $(CFLAGS) -Iinclude/
HUFFMAN_SRC := include/*.h src/comprimir.c src/descomprimir.c src/estruturas.c src/utils.c

TEST_CFLAGS := $(HUFFMAN_CFLAGS) $(shell pkgconf --cflags cunit)
TEST_LDLIBS := $(shell pkgconf --libs cunit)

.PHONY: build docs

build: build/huffman$(BINEXT) build/plotagem$(BINEXT) build/testes$(BINEXT)

build/:
	mkdir $@

build/huffman$(BINEXT): src/main.c $(HUFFMAN_SRC) | build/
	$(CC) $(HUFFMAN_CFLAGS) -o $@ $(filter %.c,$^)

build/plotagem$(BINEXT): plotagem/plotagem.c
	$(CC) $(CFLAGS) -o $@ $(filter %.c,$^)

build/testes$(BINEXT): $(HUFFMAN_SRC) src/testes.c | build/
	$(CC) $(TEST_CFLAGS) -o $@ $(filter %.c,$^) $(TEST_LDLIBS)

docs:
	doxygen
