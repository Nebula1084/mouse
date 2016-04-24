CC = gcc
CFLAGS = -Wall -g

TARGET = sample

.PHONY: clean all

all: sample

sample: sample.o mouse.o
	$(CC) -o $(TARGET) $^
sample.o: sample.c mouse.h
	$(CC) $(CFLAGS) -c $<
mouse.o: mouse.c mouse.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o
	rm -rf $(TARGET)