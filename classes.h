//====================================================
// classes.h
// by Tejas Jha, April 9, 2018
//
// classes used in performing classification of 
// features using MultiVariate Guassian Distribution
//
//====================================================

#ifndef CLASSES_H
#define CLASSES_H

#include <vector>
#include <string>

//Representation of image being looked at
class Image;
//Representation of raster layer
class Raster;
//2D Representaiton of slice of raster layer
struct Slice;

class Image {
private:
    std::vector<Raster> layers;
    std::string img_name;
    int nx;
    int ny;
    int xsize(int inx) {nx = inx;}
    int ysize(int iny) {ny = iny;}

public:
    Image() : nx(0), ny(0) {}
    int xsize(){ return nx; }s
    int ysize(){ return ny; }
    Image(const std::string &img_file); // Image initialization

    //Requires: vector of slice objects to label
    //Returns: 
    Raster set_data(const std::vector<Slice> &slices);
    Raster train(const std::vector<Raster> &rasters, const Raster &training_raster);
    
    /*
    friend ostream& operator<< (ostream &out, Image &im);
    friend istream& operator>> (istream  &in, Image &im);
    
    void  read_file(const std::string &infile);
    void  write_file(const std::string &outfile);
    void  set_train(const std::string &infile);
    */

};

class Raster {

};

struct Slice {
    int x0;
    int y0;
    int dx;
    int dy;
    int label;
};

#endif