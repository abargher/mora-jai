.PHONY: all
all: lib handler

.PHONY: lib
lib: libmora.a

.PHONE: jaios
jaios: libjaios.a

.PHONY: clean
clean:
	@echo Cleaning up all object files
	@rm -rf objs/*.o
	@rm -rf objs/lib/mora/*.o
	@rm -rf objs/lib/jaios/*.o
	@rm -rf handler libmora.a jaios.a

CFLAGS = -Wall -Werror -Wno-unknown-pragmas

_LIBS = mora.c
_LIB_PATH = ../lib/mora/
LIBS = $(patsubst %.c, $(_LIB_PATH)/%.c, $(_LIBS))

_OS = jaios.c state_puzzle.c state_puzzle_solved.c state_pack_browse.c
_OS_PATH = ../lib/jaios/
OS = $(patsubst %.c, $(_OS_PATH)/%.c, $(_OS))

_SRCS = main.c random.c pc_hardware.c
_SRC_PATH = ../pc_test/
SRCS = $(patsubst %.c, $(_SRC_PATH)%.c, $(_SRCS))

LIB_OBJS =$(patsubst $(_LIB_PATH)%.c, objs/lib/mora/%.o, $(LIBS))
OS_OBJS = $(patsubst $(_OS_PATH)%.c, objs/lib/jaios/%.o, $(OS))
SRC_OBJS = $(patsubst $(_SRC_PATH)%.c, objs/%.o, $(SRCS))
OBJS = $(LIB_OBJS) $(OS_OBJS) $(SRC_OBJS)

MORA_H = $(_LIB_PATH)/mora.h
JAIOS_H = $(_OS_PATH)/jaios.h
HARDWARE_H = ../lib/hardware/hardware.hpp
LIB_HEADERS =$(MORA_H) $(JAIOS_H) $(HARDWARE_H)

# Create explicit dependencies for every lib .o on its parent .c
$(foreach obj, $(LIB_OBJS), $(eval $(obj):$(patsubst objs/lib/mora%.o, $(_LIB_PATH)/%.c, $(obj))))
$(foreach obj, $(OS_OBJS), $(eval $(obj):$(patsubst objs/lib/jaios%.o, $(_OS_PATH)/%.c, $(obj))))
$(foreach obj, $(SRC_OBJS), $(eval $(obj):$(patsubst objs/%.o, $(_SRC_PATH)/%.c, $(obj))))

# Create dependencies for every .o on every lib header
$(foreach obj, $(OBJS), $(eval $(obj):$(LIB_HEADERS)))

# TODO: src headers, ignored for the moment

# Eradicate built-in rules for everything
.SUFFIXES:

# Overwrite built-in rule for compiling sources
objs/lib/mora/%.o : $(_LIB_PATH)/%.c
	gcc $(CFLAGS) -c $< -o $@

objs/lib/jaios/%.o : $(_OS_PATH)/%.c
	gcc $(CFLAGS) -c $< -o $@

objs/%.o : $(_SRC_PATH)/%.c
	gcc $(CFLAGS) -c $< -o $@

libmora.a: $(LIB_OBJS)
	echo $(LIB_OBJS)
	ar rcs libmora.a $(LIB_OBJS)

libjaios.a: libmora.a $(OS_OBJS)
	ar rcs libjaios.a $(OS_OBJS)
	
handler: libmora.a libjaios.a $(SRC_OBJS)
	gcc $(CFLAGS) -L. -o $@ $(SRC_OBJS) -ljaios -lmora
