sample: sample.o mouse.o
	g++ -o sample sample.o mouse.o
sample.o: sample.c mouse.h
	g++ -c sample.c
mouse.o: mouse.c mouse.h
	g++ -c mouse.c
