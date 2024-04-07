FILES = main.cpp vec3.h color.h


br: 
	g++ -O2 $(FILES) -pg -g -o main && ./main > image.ppm

i:
	eog image.ppm

bri:
	g++ $(FILES) -o main && ./main > image.ppm && eog image.ppm

len:
	find . -name '*.cpp' | xargs wc -l && find . -name '*.h' | xargs wc -l

