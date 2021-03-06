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
#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/Dense>

//Initialize Image object from provided image file
Image::Image(const std::string &img_file, int num_layers){
    //Throw error if file cannot be read
    FileOpenErrorException FileOpenError;

    // Open File Data
    GDALDataset *poDataset;
    GDALAllRegister();
    this->img_name = img_file;    
    poDataset = (GDALDataset *) GDALOpen(img_file.c_str(), GA_ReadOnly );
    if( poDataset == NULL ) throw FileOpenError;

    GDALRasterBand *poBand;

    int img_x;
    int img_y;

    // Currently can take in rasters of different sizes
    for(int i = 1; i <= num_layers; i++){
        poBand = poDataset->GetRasterBand( i ); // the first channel.
        int xsize = poBand->GetXSize();
        int ysize = poBand->GetYSize();

        Raster ras(xsize,ysize, false, false); //Create Raster
        std::vector<int> data(xsize*ysize); //Raster data

        // temporary place for one line of the image:
        int *buf = new int[xsize];

        // fill the data vector:
        int pix=0;
        for(int iy=0; iy < ysize;++iy){
            CPLErr err = poBand->RasterIO( GF_Read, 
                    0, iy, xsize, 1,
                    buf, xsize, 1, GDT_Float32,
                    0, 0 );
            for(int ix=0; ix < xsize; ++ix) {
                 data[pix++]=buf[ix];
            }//endfor: ix

        } // endfor: iy

        ras.set_data(data); //Set ras to hold data
        layers.push_back(ras);
        delete buf;

        if(i == 1){
            img_x = xsize;
            img_y = ysize;
        }
    }

    // Incorrectly stores last raster dimensions as image dimensions
    this->nx = img_x;
    this->ny = img_y;
    GDALClose(poDataset);
}

//Generate Raster data for training
Raster Image::set_training(const std::vector<Slice> &slices){
    Raster ras(nx, ny, true, false);
    ras.set_data(slices);
    return ras;
    //layers.push_back(ras);
}

//Generate Raster data for testing
Raster Image::set_testing(const std::vector<Slice> &slices){
    Raster ras(nx, ny, false, true);
    ras.set_data(slices);
    return ras;
    //layers.push_back(ras);
}

// Set data into raster object 
void Raster::set_data(const std::vector<int> &input_data){
    data = input_data;
}

// Set data into raster object 
void Raster::set_data(const std::vector<Slice> &slices){
    data.resize(nx * ny);

    // Go through all slices
    for(int i = 0; i < slices.size(); i++){
        int x = slices[i].x0; //initial x
        int y = slices[i].y0; //initial y

        // Loop through slice
        for(int iy = 0; iy < slices[i].dy; iy++){
            for(int ix = 0; ix < slices[i].dx; ix++){
                data[nx*(y+iy) + (x + ix)] = slices[i].label; // Set slice
            }
        }

    }
}

//Training 
// Assumes that raster sizes are eqivalent
void Image::train(const std::vector<Raster> &rasters, const Raster &training_raster){
    //Go through and accumulate stats
    // Need two passes at least , one for mean , one for covariance
    //Go through each data raster
    //Check if corresponds to training data
        // if yes, then add to calculate mean

    //Assert here if raster sizes are equivialent

    std::vector<Stats> stats(rasters.size());
    for (int i = 0; i < stats.size(); i++){
        stats[i].means.resize(rasters.size());
        stats[i].sums.resize(rasters.size());
        stats[i].n.resize(rasters.size());
    }
    
    const std::vector<int> train = training_raster.get_data();

    for(int i = 0; i < rasters.size(); i++){ //For each raster in data
        int *buf = new int[rasters[i].xsize()]; //Temporary line data
        const std::vector<int> vec = rasters[i].get_data();

        //Go through each line of data
        for(int iy=0; iy < rasters[i].ysize();++iy){
            for(int ix=0; ix < rasters.xsize(); ++ix) {
                int cls = train[iy*training_raster.xsize() + ix];
                //If training_raster is not 0,
                // Set corresponding raster mean 
                if( cls != 0){
                    stats.sums[cls] += vec[iy*rasters[i].ysize() + ix];
                    stats.n[cls] += 1;
                }

            }//endfor: ix

        } // endfor: iy

        //Set means
        for(int p = 0; p < stats.means.size(); p++){
            stats[i].means[p] = (long double)stats[i].sums[p] / (long double)stats[i].n[p];
        }

    }

    //Loop through again to calculate covariance
        
}