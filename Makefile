CC := g++
CFLAGS_DEBUG   := -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -pie -Wlarger-than=8192 -Wstack-usage=8192

CFLAGS_RELEASE := -D NDEBUG -D NVERIFY -std=c++20 -O2
CFLAGS_VERIFY  := $(CFLAGS_DEBUG) -D NDEBUG
CFLAGS_DEBUG   := $(CFLAGS_DEBUG) -D _DEBUG

ifeq ($(mode), debug)
CFLAGS := $(CFLAGS_DEBUG)
else ifeq ($(mode), verify)
CFLAGS := $(CFLAGS_VERIFY)
else
CFLAGS ?= $(CFLAGS_RELEASE)
endif

#--------------------------------------------------------------------------------------------------------------------------------

BUILD_DIR  ?= build/
LIB_PREFIX ?= ./

include include_paths.d

#================================================================================================================================


all:


.PHONY: compilation_database
compilation_database:
	$(MAKE) clean
	bear -- $(MAKE) all -k

$(BUILD_DIR)algorithm.o: $(LIB_PREFIX)algorithm/src/algorithm.cpp | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@
all: $(BUILD_DIR)algorithm.o

$(BUILD_DIR)array.o: $(LIB_PREFIX)array/src/array.cpp | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@
all: $(BUILD_DIR)array.o

#$(BUILD_DIR)/cache_list.o: $(LIB_PREFIX)cache_friendly_list/src/cache_list.cpp | $(BUILD_DIR)
#	$(CC) -c $(CFLAGS) $< -o $@
#all: $(BUILD_DIR)/cache_list.o

$(BUILD_DIR)list.o: $(LIB_PREFIX)list/src/list.cpp | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@
all: $(BUILD_DIR)list.o

$(BUILD_DIR)log.o: $(LIB_PREFIX)log/src/log.cpp | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@
all: $(BUILD_DIR)log.o

$(BUILD_DIR)trace.o: $(LIB_PREFIX)log/src/trace.cpp | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@
all: $(BUILD_DIR)trace.o

$(BUILD_DIR)stack.o: $(LIB_PREFIX)stack/src/stack.cpp | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@
all: $(BUILD_DIR)stack.o

$(BUILD_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
