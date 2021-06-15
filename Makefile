CC = gcc
CFLAGS = -Wall -Werror=format-security -Werror=implicit-function-declaration -Wextra

BDIR = build
ODIR = $(BDIR)/obj

TEST_COMPONENTS = $(shell ls src/tests)
TESTBDIR = $(BDIR)/tests
TESTTGTS = $(patsubst %,$(TESTBDIR)/%,$(TEST_COMPONENTS))

define test_src
src/tests/$(shell echo $1 | cut -d"/" -f 3)/main.c
endef

INCLUDES = src/include

_OBJS = $(shell ls src/*.c)
OBJS = $(patsubst src/%.c,$(ODIR)/%.o,$(_OBJS))


$(ODIR)/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< -I$(INCLUDES) $(CFLAGS)

.PHONY: all

all: tests

.PHONY: debug 
debug: CFLAGS += -D DEBUG
debug: all

	
tests: $(TESTTGTS)
	
$(TESTBDIR)/%: $(OBJS)
	@mkdir -p $(TESTBDIR)
	$(CC) -o $@ $(call test_src,$@) $(OBJS) -I$(INCLUDES) $(CFLAGS)
	
.PRECIOUS: $(ODIR)/%.o

.PHONY: clean
clean:
	rm -rf $(BDIR)

