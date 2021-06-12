CC = gcc
CFLAGS = -Wall -g -Werror=format-security -Werror=implicit-function-declaration -Wextra

BDIR = build
ODIR = $(BDIR)/obj

COMPONENT_ROOT := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))/..
INCLUDES = $(COMPONENT_ROOT)/include

DEPS = scanner.h

_OBJ = scanner.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< -I$(INCLUDES) $(CFLAGS)

$(BDIR)/main: main.c $(OBJ)
	$(CC) -o $@ $^  -I$(INCLUDES) $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(BDIR)

