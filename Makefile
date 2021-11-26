all: Grapher.cpp main.cpp
	g++ -o graph main.cpp Grapher.cpp -lraylib
