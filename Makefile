CC ?= g++
CFLAGS_DEBUG   := -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -pie -Wlarger-than=8192 -Wstack-usage=8192
CFLAGS_RELEASE := -std=c++20 -O2

ifeq ($(mode), debug)
CFLAGS ?= $(CFLAGS_DEBUG)
CFLAGS := $(CFLAGS) -D _DEBUG
else ifeq ($(mode), verify)
CFLAGS ?= $(CFLAGS_DEBUG)
CFLAGS := $(CFLAGS) -D NDEBUG
else
CFLAGS ?= $(CFLAGS_RELEASE)
CFLAGS := $(CFLAGS) -D NDEBUG -D NVERIFY
endif

#--------------------------------------------------------------------------------------------------------------------------------

BUILD_DIR   ?= build/
ROOT_PREFIX ?= ./

PREFIX   ?= $(ROOT_PREFIX)
INCLUDES := -I$(PREFIX)include/

#================================================================================================================================

.PHONY: all
all:;


SRCS := $(PREFIX)algorithm/src/algorithm.cpp \
        $(PREFIX)array/src/array.cpp         \
        $(PREFIX)list/src/list.cpp           \
        $(PREFIX)log/src/log.cpp             \
        $(PREFIX)log/src/trace.cpp           \
        $(PREFIX)stack/src/stack.cpp
SRCS := $(patsubst $(ROOT_PREFIX)%.cpp, %.cpp, $(SRCS))
OBJS := $(patsubst %.cpp, $(BUILD_DIR)%.o, $(SRCS))
DEPS := $(patsubst %.o, %.d, $(OBJS))

$(DEPS): $(BUILD_DIR)%.d: $(ROOT_PREFIX)%.cpp
	mkdir -p $(dir $@)
	$(CC) $< $(CFLAGS) $(INCLUDES) -MM -MT '$(BUILD_DIR)$*.o $(BUILD_DIR)$*.d' -MF $@
include $(DEPS)

$(OBJS): $(BUILD_DIR)%.o: $(ROOT_PREFIX)%.cpp
	$(CC) $< $(CFLAGS) $(INCLUDES) -c -o $@
all: $(OBJS)
