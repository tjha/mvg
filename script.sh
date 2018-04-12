g++ -c -g -std=c++0x -o class_init.o class_init.cpp -I /home/tjha/libraries/eigen-eigen-5a0156e40feb/ `gdal-config --cflags` `gdal-config --libs` `gdal-config --dep-libs` 
g++ -g -std=c++0x -o prog source.cpp class_init.o -I /home/tjha/libraries/eigen-eigen-5a0156e40feb/ `gdal-config -DD-cflags` `gdal-config --libs` `gdal-config --dep-libs`
