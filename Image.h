//================================================
// Image.h
//
// by Leland Pierce, Jan 13, 2016
// Adapted By Tejas Jha Mar 30, 2018
//
// Header file for the Image class.
//================================================
#ifndef IMAGE_H
#define IMAGE_H


#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;


//-----------------------------------------------------------------
// Image class:
// summary: An image is a 2D array of doubles.
//          This class provides a default constructor, along with
//          a way to read an image from a stream, write an image to a stream,
//          and a way to filter an image to create another image.
// typical usage:
//          #include "Image.h"
//          create an empty image:         Image im1;
//          read image data from a stream: instream >> im1;
//          write image to a stream:       outstream << im1;
//          threshhold an image (set anything <=4 to zero)
//                               Image im2;
//                               im2 = im1.filter("threshhold",4.);
//          obtain image dimensions:
//                               int nx = im1.xsize();
//                               int ny = im1.ysize();
// caveats:
//          So far there are no public methods for getting or setting
//          the image data.
//          Only implements single-channel images.
//
// internals:
//          This class uses 3 private instance variables for maintaining
//          the state of the object:
//          2 integers, keeping track of the size in each dimension: nx, ny
//          a vector of doubles, for storing a 1D representation of the
//            image data.
//            indexing is like: [iy*nx + ix] where we want (ix,iy)
//          The user is not allowed to set the dimensions.
//          Currently there is no way for the user to set or get
//            values within the image (pixels).
//
//-----------------------------------------------------------------


class Image {
    
private:
    vector<double> data;
    int nx;
    int ny;
    int xsize(int inx) {nx = inx;}
    int ysize(int iny) {ny = iny;}
    
public:
    Image() : nx(0), ny(0) {}
    void filter(std::string filter_name, double param);
    int xsize(){ return nx; }
    int ysize(){ return ny; }
    // use default destructor
    
    friend ostream& operator<< (ostream &out, Image &im);
    friend istream& operator>> (istream  &in, Image &im);
    
    void  read_file(const std::string &infile);
    void  write_file(const std::string &outfile);
    void  set_train(const std::string &infile);

    
};// end class Image


// Raster Class
class Raster {
private:
    bool data;
    bool train;
    bool test;

public:
};

struct Slice {
    vector<double> temp_data;
    int start_x;
    int start_y;
    int dx;
    int dy;
};

#endif
