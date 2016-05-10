RM      := rm
CC      := clang
LD      := clang
CFLAGS  := -std=gnu99 -Wall -pedantic -O2 -march=native
LDFLAGS :=
SOURCES := $(shell find . -name "*.c" -type f -print)
OBJECTS := $(SOURCES:%.c=%.o)
DEPS    := $(OBJECTS:%.o=%.d)
PROG    := hex

all: $(PROG)

$(PROG): $(OBJECTS)
	@echo "LD		$@"
	@$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	@echo "CC		$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

%.d: %.c
	@echo "DEP		$@"
	@$(CC) $(CFLAGS) -MM -MF $@ -MT $*.o $<

clean:
	@echo "CLEAN"
	@$(RM) -f *.o *.d main

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

.PHONY: all clean
