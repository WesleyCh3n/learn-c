CC = gcc
CFLAGS = -std=gnu99
CFLAGS += -Wall -Wextra -Werror -pedantic
CFLAGS += -m64

SOURCES =
HEADERS = $(SOURCES:.c=.h)

PROGRAMS = ./pip_exp.c ./subprocess.c ./pip_process.c
OBJECTS = $(PROGRAMS:.c=.o) $(SOURCES:.c=.o)
TARGETS = $(PROGRAMS:.c=)

all: $(TARGETS)

$(TARGETS): $(OBJECTS)
	$(Q)$(CC) -o $@ $(CFLAGS) $(@:=.o)

%.o: %.c
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(TARGETS)
