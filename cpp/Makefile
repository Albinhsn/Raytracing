FILES = main.cpp vec3.h color.h


br: 
	g++ $(FILES) -o main && ./main > image.ppm

i:
	eog image.ppm

bri:
	g++ $(FILES) -o main && ./main > image.ppm && eog image.ppm
