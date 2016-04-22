CC = gcc
CC_FLAG = -Wall

TARGET = sample

.PHONY: clean all

all: sample

sample: sample.o mouse.o
	$(CC) -o $(TARGET) $^
sample.o: sample.c mouse.h
	$(CC) $(CC_FLAG) -c $<
mouse.o: mouse.c mouse.h
	$(CC) $(CC_FLAG) -c $<

clean:
	rm -rf *.o
	rm -rf $(TARGET)