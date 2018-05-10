MINI = mini_morus
MEDIUM = medium_morus
FULL = morus
LIBS = -lm -march=native -g #-fopenmp #-O0 -fsanitize=address
CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native -g #-fopenmp #-fsanitize=address -O0
COMMON_FILE = aes-rng.c
MINI_OBJECTS = $(MINI:%=%.o) $(MINI:%=main_%.o) $(COMMON_FILE:%.c=%.o)
MEDIUM_OBJECTS = $(MEDIUM:%=%.o) $(MEDIUM:%=main_%.o) $(COMMON_FILE:%.c=%.o)
FULL_OBJECTS = $(FULL:%=%.o) $(FULL:%=main_%.o) $(COMMON_FILE:%.c=%.o)
HEADERS = $(wildcard *.h)

.PHONY: default all clean

default: $(MINI) $(MEDIUM) $(FULL)
all: default

%.o: %.c $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(MINI) $(MINI_OBJECTS) $(MEDIUM) $(MEDIUM_OBJECTS) $(FULL) $(FULL_OBJECTS)

$(MINI): $(MINI_OBJECTS)
		$(CC) $(MINI_OBJECTS) -Wall $(LIBS) -o $@

$(MEDIUM): $(MEDIUM_OBJECTS)
		$(CC) $(MEDIUM_OBJECTS) -Wall $(LIBS) -o $@

$(FULL): $(FULL_OBJECTS)
		$(CC) $(FULL_OBJECTS) -Wall $(LIBS) -o $@

clean:
		-rm -f *.o
		-rm -f $(FULL)
		-rm -f $(MEDIUM)
		-rm -f $(MINI)
