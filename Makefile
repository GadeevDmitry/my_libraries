CC ?= g++
CFLAGS ?= -D NDEBUG -std=c++20 -O2

COMMON_PREFIX ?=

#================================================================================================================================


all:


build/common/algorithm.o: $(COMMON_PREFIX)/algorithm/algorithm.cpp | build/common
	$(CC) -c $(CFLAGS) $< -o $@
all: build/common/algorithm.o

build/common/array.o: $(COMMON_PREFIX)/array/array.cpp | build/common
	$(CC) -c $(CFLAGS) $< -o $@
all: build/common/array.o

build/common/cache_list.o: $(COMMON_PREFIX)/cache_friendly_list/cache_list.cpp | build/common
	$(CC) -c $(CFLAGS) $< -o $@
all: build/common/cache_list.o

build/common/list.o: $(COMMON_PREFIX)/list/list.cpp | build/common
	$(CC) -c $(CFLAGS) $< -o $@
all: build/common/list.o

build/common/logs.o: $(COMMON_PREFIX)/logs/log.cpp | build/common
	$(CC) -c $(CFLAGS) $< -o $@
all: build/common/logs.o

build/common/trace.o: $(COMMON_PREFIX)/logs/trace/trace.cpp | build/common
	$(CC) -c $(CFLAGS) $< -o $@
all: build/common/trace.o

build/common/stack.o: $(COMMON_PREFIX)/stack/stack.cpp | build/common
	$(CC) -c $(CFLAGS) $< -o $@
all: build/common/stack.o

build/common:
	mkdir -p $@
