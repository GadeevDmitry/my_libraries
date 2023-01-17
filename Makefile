CFLAGS = -D _DEBUG -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -pie -Wlarger-than=8192 -Wstack-usage=8192

LOG     = logs/log
LOG_H   = $(LOG).h $(LOG)_static.h
LOG_O	= $(LOG).o
LOG_CPP = $(LOG).cpp

$(LOG_O): $(LOG_CPP) $(LOG_H)
	g++ -c $(CFLAGS) $< -o $@

ALG     = algorithm/algorithm
ALG_H	= $(ALG).h $(ALG)_static.h
ALG_O	= $(ALG).o
ALG_CPP = $(ALG).cpp

$(ALG_O): $(ALG_CPP) $(ALG_H)
	g++ -c $(CFLAGS) $< -o $@

STK     = stack/stack
STK_H   = $(STK).h $(STK)_static.h
STK_O   = $(STK).o
STK_CPP = $(STK).cpp

$(STK_O): $(STK_CPP) $(STK_H)
	g++ -c $(CFLAGS) $< -o $@

LST     = list/list
LST_H   = $(LST).h $(LST)_static.h
LST_O   = $(LST).o
LST_CPP = $(LST).cpp

$(LST_O): $(LST_CPP) $(LST_H)
	g++ -c $(CFLAGS) $< -o $@
