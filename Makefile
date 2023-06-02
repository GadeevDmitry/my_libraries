CFLAGS = -D _DEBUG -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -pie -Wlarger-than=8192 -Wstack-usage=8192
#--------------------------------------------------------------------------------------------------------------------------------
TRC	      = logs/trace/trace
LOG       = logs/log
ALG       = algorithm/algorithm
ARR       = array/array
STK       = stack/stack
VEC       = vector/vector
LST       = list/list
CACHE_LST = cache_friendly_list/cache_list
#--------------------------------------------------------------------------------------------------------------------------------
TRC_H      = $(TRC).h
TRC_STAT_H = $(TRC)_static.h $(TRC_H)  $(TRC)_settings.h
TRC_SRC    = $(TRC).cpp $(TRC_STAT_H)
#--------------------------------------------------------------------------------------------------------------------------------
LOG_H      = $(LOG).h $(LOG)_settings.h $(LOG)_def.h
LOG_STAT_H = $(LOG)_static.h $(LOG_H) $(TRC_H)
LOG_SRC    = $(LOG).cpp $(LOG_STAT_H)
#--------------------------------------------------------------------------------------------------------------------------------
ALG_H      = $(ALG).h $(LOG_H)
ALG_STAT_H = $(ALG)_static.h $(ALG_H) $(ALG)_settings.h
ALG_SRC    = $(ALG).cpp $(ALG_STAT_H)
#--------------------------------------------------------------------------------------------------------------------------------
ARR_H      = $(ARR).h $(LOG_H) $(ALG_H)
ARR_STAT_H = $(ARR)_static.h $(ARR_H) $(ARR)_settings.h
ARR_SRC    = $(ARR).cpp $(ARR_STAT_H)
#--------------------------------------------------------------------------------------------------------------------------------
STK_H      = $(STK).h $(LOG_H) $(ALG_H)
VEC_H      = $(VEC).h $(STK_H)

STK_STAT_H = $(STK)_static.h $(STK)_settings.h $(STK_H)
VEC_STAT_H = $(VEC)_static.h $(VEC_H) $(STK_STAT_H)

STK_SRC    = $(STK).cpp $(STK_STAT_H) $(VEC_STAT_H)
#--------------------------------------------------------------------------------------------------------------------------------
LST_H      = $(LST).h $(LOG_H) $(ALG_H)
LST_STAT_H = $(LST)_static.h $(LST_H) $(LST)_settings.h
LST_SRC    = $(LST).cpp $(LST_STAT_H)
#--------------------------------------------------------------------------------------------------------------------------------
CACHE_LST_H      = $(CACHE_LST).h $(LOG_H) $(ALG_H)
CACHE_LST_STAT_H = $(CACHE_LST)_static.h $(CACHE_LST_H) $(CACHE_LST)_settings.h
CACHE_LST_SRC    = $(CACHE_LST).cpp $(CACHE_LST_STAT_H)
#--------------------------------------------------------------------------------------------------------------------------------

$(TRC).o      : $(TRC_SRC)
$(LOG).o      : $(LOG_SRC)
$(ALG).o      : $(ALG_SRC)
$(ARR).o      : $(ARR_SRC)
$(STK).o      : $(STK_SRC)
$(LST).o      : $(LST_SRC)
$(CACHE_LST).o: $(CACHE_LST_SRC)

all  : $(TRC).o $(LOG).o $(ALG).o $(ARR).o $(STK).o $(LST).o $(CACHE_LST).o
clean:
	rm -f $(TRC).o
	rm -f $(LOG).o
	rm -f $(ALG).o
	rm -f $(ARR).o
	rm -f $(STK).o
	rm -f $(LST).o
	rm -f $(CACHE_LST).o

.PHONY: all clean

%.o: %.cpp
	g++ -c $(CFLAGS) $< -o $@
