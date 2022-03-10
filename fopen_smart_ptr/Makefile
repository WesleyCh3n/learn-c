target = fopen_smart_ptr

CC = g++
CFLAGS =
CFLAGS += -std=c++14 -O3
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -m32

VAR := $(shell cat version.h | grep 'MyVer' | grep -Eo '[0-9]+[.]?[0-9]+[.]?[0-9]+')

all: $(target)

$(target): $(target).o
	$(Q)echo "Compiling version $(VAR)"
	$(Q)$(CC) -o $@ $(CFLAGS) $<

%.o: %.cc
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -rf *.o $(target)
