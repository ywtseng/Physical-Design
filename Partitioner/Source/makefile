EXE = fm

all: main.o 
	g++ -Wall  -o $(EXE) main.o

main.o: main.cpp 
	g++ -Wall  -c main.cpp

clean:
	rm $(EXE) *.o
