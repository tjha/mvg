//=================================================
// class_init.cpp
// By Tejas Jha, April 9 2018
//
// Implementation of class data configuration
//=================================================

#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include "Classes.h"
#include "Exceptions.hpp"
// Below #includes may need adjustments
#include "Exceptions.hpp"
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <ogr_spatialref.h>

//Initialize Image object from provided image file
Image::Image(const std::string &img_file, int num_layers){
    //Throw error if file cannot be read
    FileOpenErrorException FileOpenError;

    // Open File Data
    GDALDataset *poDataset;
    GDALAllRegister();
    this->img_name = img_file;
    poDataset = (GDALDataset *) GDALOpen(cname, GA_ReadOnly );
    if( poDataset == NULL ) throw FileOpenError;

    GDALRasterBand *poBand;

    for(int i = 1; i <= num_layers; i++){
        poBand = poDataset->GetRasterBand( i ); // the first channel.
        int xsize = poBand->GetXSize();
        int ysize = poBand->GetYSize();

        Raster ras(xsize,ysize, false, false); //Create Raster
        vector<int> data(xsize*ysize); //Raster data

        ras.set_data(data); //Set ras to hold data
        layers.push_back(ras);
    }

    this->nx = x;
    this->ny = y;
}

//Generate Raster data for training
void Image::set_training(const std::vector<Slice> &slices){
    Raster rast(nx, ny, true, false);
    rast.set_data(slices);
}

//Generate Raster data for testing
void Image::set_testing(const std::vector<Slice> &slices){
    Raster rast(nx, ny, false, true);
    rast.set_data(slices);
}
