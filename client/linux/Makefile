CC = g++
CFLAGS = -std=c++11
DEPENDENCIES = -lXtst -lX11
EXE_NAME = SimulateInput

default: input

input: main.o InputSimulator.o
		$(CC) $(CFLAGS) -o $(EXE_NAME) main.o InputSimulator.o $(DEPENDENCIES)

main.o: main.cpp InputSimulator.h
		$(CC) $(CFLAGS) -c main.cpp

InputSimulator.o: InputSimulator.cpp InputSimulator.h
		$(CC) $(CFLAGS) -c InputSimulator.cpp


clean:
		$(RM) $(EXE_NAME) *.o
