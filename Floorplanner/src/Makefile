EXE = btree

all: main.cpp btree.h parser.cpp btree.cpp contour.cpp sa.cpp
	g++ -o $(EXE) main.cpp parser.cpp btree.cpp contour.cpp sa.cpp

%.o: %.cpp
	g++ -c $<

clean:
	rm $(EXE) 
