LIB_PREFIX ?= ./

CFLAGS += -I$(LIB_PREFIX)log/include -I$(LIB_PREFIX)stack/include -I$(LIB_PREFIX)vector/include \
          -I$(LIB_PREFIX)list/include -I$(LIB_PREFIX)array/include -I$(LIB_PREFIX)algorithm/include
