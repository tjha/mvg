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
    Image(const std::string &img_file, int num_layers); // Image initialization
    int xsize(){ return nx; }
    int ysize(){ return ny; }
    const std::vector<Raster> &get_layers() { return layers; }

    // Requires:  vector of slice objects to label
    // Modifies:  Raster object with labels at provided 
    //            locations and 0s eleswhere is added
    //            to vector list layers
    Raster set_training(const std::vector<Slice> &slices);
    Raster set_testing(const std::vector<Slice> &slices);
    void train(const std::vector<Raster> &rasters, const Raster &training_raster);
    void push_back(const Raster &raster) { layers.push_back(raster); }
    
    /*
    friend ostream& operator<< (ostream &out, Image &im);
    friend istream& operator>> (istream  &in, Image &im);
    
    void  read_file(const std::string &infile);
    void  write_file(const std::string &outfile);
    void  set_train(const std::string &infile);s
    */

};

//Stats to create a k-dimensional Guassian PDF
//for each classification.
// Likeley going to be a nested struct in image
// private
struct Stats {
    std::vector<double> means;
    std::vector<long double> sums;
    std::vector<int> n;
    std::vector<double> z;
};

class Raster {
private:
    std::vector<int> data;
    int nx;
    int ny;
    bool train;
    bool test;
public:
    Raster(): nx(0), ny(0) {}
    Raster(int inx, int iny, bool train_in, bool test_in): nx(inx), ny(iny), train(train_in), test(test_in) { }
    int xsize() { return nx; }
    int ysize() { return ny; }
    bool is_train() { return train; }
    bool is_test() { return test; }
    void set_data(const std::vector<Slice> &slices);
    void set_data(const std::vector<int> &input_data);
    const std::vector<int> &get_data(){ return data;} //Mainly for error checking for now
};

struct Slice {
    int x0;
    int y0;
    int dx;
    int dy;
    int label;
};

#endif