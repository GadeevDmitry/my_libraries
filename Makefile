CFLAGS = -D _DEBUG -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -pie -Wlarger-than=8192 -Wstack-usage=8192

LOG     = logs/log
ALG     = algorithm/algorithm
STK     = stack/stack
LST     = list/list

LOG_H   = $(LOG).h $(LOG)_static.h $(LOG)_def.h $(LOG)_undef.h $(LOG)_settings.h
ALG_H	= $(ALG).h $(ALG)_static.h $(ALG)_def.h $(ALG)_undef.h
STK_H   = $(STK).h $(STK)_static.h $(STK)_def.h $(STK)_undef.h $(STK)_settings.h
LST_H   = $(LST).h $(LST)_static.h $(LST)_def.h $(LST)_undef.h $(LST)_settings.h

LOG_O	= $(LOG).o
ALG_O	= $(ALG).o
STK_O   = $(STK).o
LST_O   = $(LST).o

$(LOG_O): $(LOG_H)
$(ALG_O): $(ALG_H) $(LOG_H)
$(STK_O): $(STK_H) $(LOG_H) $(ALG_H)
$(LST_O): $(LST_H) $(LOG_H) $(ALG_H)

%.o: %.cpp
	g++ -c $(CFLAGS) $< -o $@

clean:
	rm -f $(LOG_O)
	rm -f $(ALG_O)
	rm -f $(STK_O)
	rm -f $(LST_O)

.PHONY: clean