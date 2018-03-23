MINI = mini_morus
FULL = morus
LIBS = -lm -fopenmp
CC = gcc
CFLAGS = -g -Wall -O3 -fopenmp
MINI_OBJECTS = $(MINI:%=%.o) $(MINI:%=main_%.o)
FULL_OBJECTS = $(FULL:%=%.o) $(FULL:%=main_%.o)
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
