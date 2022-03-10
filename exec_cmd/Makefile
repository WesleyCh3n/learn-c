target = exec_cmd

CC = g++
CFLAGS =
CFLAGS += -std=c++14 -O3
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -m64

all: $(target)

$(target): $(target).o
	$(Q)$(CC) -o $@ $(CFLAGS) $<

%.o: %.cc
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -rf *.o $(target)
