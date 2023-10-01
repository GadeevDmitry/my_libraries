CC := g++
CFLAGS_DEBUG   ?= -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -pie -Wlarger-than=8192 -Wstack-usage=8192
CFLAGS_RELEASE ?= -std=c++20 -O2

ifeq ($(mode), debug)
CFLAGS ?= $(CFLAGS_DEBUG)
CFLAGS := $(CFLAGS) -D _DEBUG
else ifeq ($(mode), verify)
CFLAGS ?= $(CFLAGS_DEBUG)
CFLAGS := $(CFLAGS) -D NDEBUG
else
CFLAGS ?= $(CFLAGS_RELEASE)
CFLAGS := $(CFLAGS) -D NDEBUG -D VVERIFY
endif

#--------------------------------------------------------------------------------------------------------------------------------

LIB_BUILD_DIR ?= build/
LIB_PREFIX    ?= ./

#================================================================================================================================


all:


.PHONY: compilation_database
compilation_database:
	$(MAKE) clean
	bear -- $(MAKE) all -k

$(LIB_BUILD_DIR)algorithm.o: $(LIB_PREFIX)algorithm/src/algorithm.cpp | $(LIB_BUILD_DIR)
	$(CC) -c $(CFLAGS) -I$(LIB_PREFIX)include/ $< -o $@
all: $(LIB_BUILD_DIR)algorithm.o

$(LIB_BUILD_DIR)array.o: $(LIB_PREFIX)array/src/array.cpp | $(LIB_BUILD_DIR)
	$(CC) -c $(CFLAGS) -I$(LIB_PREFIX)include/ $< -o $@
all: $(LIB_BUILD_DIR)array.o

#$(LIB_BUILD_DIR)cache_list.o: $(LIB_PREFIX)cache_friendly_list/src/cache_list.cpp | $(LIB_BUILD_DIR)
#	$(CC) -c $(CFLAGS) -I$(LIB_PREFIX)include/ $< -o $@
#all: $(LIB_BUILD_DIR)/cache_list.o

$(LIB_BUILD_DIR)list.o: $(LIB_PREFIX)list/src/list.cpp | $(LIB_BUILD_DIR)
	$(CC) -c $(CFLAGS) -I$(LIB_PREFIX)include/ $< -o $@
all: $(LIB_BUILD_DIR)list.o

$(LIB_BUILD_DIR)log.o: $(LIB_PREFIX)log/src/log.cpp | $(LIB_BUILD_DIR)
	$(CC) -c $(CFLAGS) -I$(LIB_PREFIX)include/ $< -o $@
all: $(LIB_BUILD_DIR)log.o

$(LIB_BUILD_DIR)trace.o: $(LIB_PREFIX)log/src/trace.cpp | $(LIB_BUILD_DIR)
	$(CC) -c $(CFLAGS) -I$(LIB_PREFIX)include/ $< -o $@
all: $(LIB_BUILD_DIR)trace.o

$(LIB_BUILD_DIR)stack.o: $(LIB_PREFIX)stack/src/stack.cpp | $(LIB_BUILD_DIR)
	$(CC) -c $(CFLAGS) -I$(LIB_PREFIX)include/ $< -o $@
all: $(LIB_BUILD_DIR)stack.o

$(LIB_BUILD_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(LIB_BUILD_DIR)
