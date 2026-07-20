.PHONY: all
all: lib handler

.PHONY: lib
lib: libmora.a


.PHONY: clean
clean:
	@echo Cleaning up all object files
	@rm -rf objs/*.o
	@rm -rf objs/lib/*.o
	@rm -rf handler libmora.a

CFLAGS = -Wall -Werror

_LIBS = mora.c
_LIB_PATH = ../lib/
LIBS = $(patsubst %.c, $(_LIB_PATH)/%.c, $(_LIBS))

_SRCS = main.c
_SRC_PATH = ../pc_test/
SRCS = $(patsubst %.c, $(_SRC_PATH)%.c, $(_SRCS))

LIB_OBJS =$(patsubst $(_LIB_PATH)%.c, objs/lib/%.o, $(LIBS))
SRC_OBJS = $(patsubst $(_SRC_PATH)%.c, objs/%.o, $(SRCS))
OBJS = $(LIB_OBJS) $(SRC_OBJS)

_LIB_HEADERS = mora.h
LIB_HEADERS = $(patsubst %, $(_LIB_PATH)%, $(_LIB_HEADERS))

# Create explicit dependencies for every lib .o on its parent .c
$(foreach obj, $(LIB_OBJS), $(eval $(obj):$(patsubst objs/lib/%.o, $(_LIB_PATH)/%.c, $(obj))))
$(foreach obj, $(SRC_OBJS), $(eval $(obj):$(patsubst objs/%.o, $(_SRC_PATH)/%.c, $(obj))))

# Create dependencies for every .o on every lib header
$(foreach obj, $(OBJS), $(eval $(obj):$(LIB_HEADERS)))

# TODO: src headers, ignored for the moment

# Eradicate built-in rules for everything
.SUFFIXES:

# Overwrite built-in rule for compiling sources
objs/lib/%.o : $(_LIB_PATH)/%.c
	gcc $(CFLAGS) $($(@)_CFLAGS) -c $< -o $@

objs/%.o : $(_SRC_PATH)/%.c
	gcc $(CFLAGS) $($(@)_CFLAGS) -c $< -o $@

libmora.a: $(LIB_OBJS)
	ar rcs libmora.a $(LIB_OBJS)
	
handler: libmora.a $(SRC_OBJS)
	gcc $(CFLAGS) -L. -o $@ $(SRC_OBJS) -lmora
