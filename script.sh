g++ -c -g -std=c++0x -o class_init.o class_init.cpp `gdal-config --cflags` `gdal-config --libs` `gdal-config --dep-libs`
g++ -g -std=c++0x -o prog source.cpp class_init.o `gdal-config -DD-cflags` `gdal-config --libs` `gdal-config --dep-libs`
