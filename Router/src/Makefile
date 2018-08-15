EXE = router

all: main.cpp router.h parser.cpp kruskal.cpp output.cpp
	g++ -o $(EXE) main.cpp parser.cpp kruskal.cpp output.cpp

%.o: %.cpp
	g++ -c $<

clean:
	rm $(EXE) 