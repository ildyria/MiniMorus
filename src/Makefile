MINI = mini_morus
FULL = morus
LIBS = -lm -fopenmp -flto -fwhole-program -fdelete-null-pointer-checks -march=native
CC = gcc
CFLAGS = -Wall -O3 -fopenmp -fdelete-null-pointer-checks -flto -march=native
COMMON_FILE = aes-rng.c
MINI_OBJECTS = $(MINI:%=%.o) $(MINI:%=main_%.o) $(COMMON_FILE:%.c=%.o)
FULL_OBJECTS = $(FULL:%=%.o) $(FULL:%=main_%.o) $(COMMON_FILE:%.c=%.o)
HEADERS = $(wildcard *.h)

.PHONY: default all clean

default: $(FULL) $(MINI)
all: default

%.o: %.c $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(FULL) $(MINI) $(MINI_OBJECTS) $(FULL_OBJECTS)

$(MINI): $(MINI_OBJECTS)
		$(CC) $(MINI_OBJECTS) -Wall $(LIBS) -o $@

$(FULL): $(FULL_OBJECTS)
		$(CC) $(FULL_OBJECTS) -Wall $(LIBS) -o $@

clean:
		-rm -f *.o
		-rm -f $(FULL)
		-rm -f $(MINI)
