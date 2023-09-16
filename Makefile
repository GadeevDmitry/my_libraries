CC ?= g++
CFLAGS ?= -D NDEBUG -std=c++20 -O2

BUILD_PREFIX := build/common

#================================================================================================================================


all:


$(BUILD_PREFIX)/list.o: $(COMMON_PREFIX)/list/list.cpp | $(BUILD_PREFIX)
	$(CC) -c $(CFLAGS) $< -o $@
all: $(BUILD_PREFIX)/list.o

$(BUILD_PREFIX)/algorithm.o: $(COMMON_PREFIX)/algorithm/algorithm.cpp | $(BUILD_PREFIX)
	$(CC) -c $(CFLAGS) $< -o $@
all: $(BUILD_PREFIX)/algorithm.o

$(BUILD_PREFIX)/logs.o: $(COMMON_PREFIX)/logs/log.cpp | $(BUILD_PREFIX)
	$(CC) -c $(CFLAGS) $< -o $@
all: $(BUILD_PREFIX)/logs.o

$(BUILD_PREFIX)/trace.o: $(COMMON_PREFIX)/logs/trace/trace.cpp | $(BUILD_PREFIX)
	$(CC) -c $(CFLAGS) $< -o $@
all: $(BUILD_PREFIX)/trace.o

$(BUILD_PREFIX):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BUILD_PREFIX)
