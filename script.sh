g++ -c -g -std=c++0x -o Image.o Image.cpp `gdal-config --cflags` `gdal-config --libs` `gdal-config --dep-libs`
g++ -g -std=c++0x -o main main.cpp Image.o `gdal-config -DD-cflags` `gdal-config --libs` `gdal-config --dep-libs`
