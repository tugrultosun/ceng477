src = *.cpp

all:
	g++ $(src) -std=c++11 -g -O3 -o raytracer
							