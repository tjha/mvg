//================================================
// Image.cpp
//
// by Leland Pierce, Jan 18, 2018
// Adapted by Tejas Jha Mar 30, 2018
//
// Implementation of the Image class.
//================================================
    
   
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <stdexcept>
	
#include "Image.h"
#include "Exceptions.hpp"
 
using namespace std;


#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <ogr_spatialref.h>

#define DEBUG

    
//-----------------------------------------------------------------
// Image constructor using input file:
// input parameter:   in: an already open istream
// summary: reads int or double ascii data from input stream
//          assumes data is arranged as ny rows each with nx numbers in it.
//          throws bad_exception if:
//               any data is not numbers
//               any row has a different number of values than the first
//               the istream is invalid
//               if there is no data
//-----------------------------------------------------------------
    istream &operator>>( istream  &in, Image &im )
    {

        string input_line;
        int input_line_number=0;
        double d;
        
        // read input file line-by-line:
        in.exceptions ( std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit );
        try {
            getline(in, input_line);
        }
        catch (...) {
            cerr << "Error in Image reader: reading from the input stream failed."<<endl;
            throw bad_exception();
        }// end-catch
        
#ifdef DEBUG
        cerr << "at 3.2"<<endl;
#endif
        
        while(!in.fail() && !in.eof() ){
            // parse the line assuming its a bunch of numbers
            size_t idx=0;
            int count_this_line = 0;
            string input_piece = input_line;
            
#ifdef DEBUG
            cerr << "at 3.3: input_piece=|"<<input_piece<<"|"<<endl;
#endif
            
            // look for next number,
            // stopping when the rest of the string is empty or has no digits
            while(!input_piece.empty() &&
                  input_piece.find_first_of("0123456789") != string::npos ){
#ifdef DEBUG
                cerr << "at 3.4: input_piece=|"<<input_piece<<"|"<<endl;
#endif
                try {
                    // parse next number from the input line:
                    d = stod(input_piece, &idx);
                }
                catch(...){
                    cerr << "Error in Image reader: converting to double."<<endl;
                    cerr << "                input: "<< input_line << endl;
                    throw bad_exception();
                }// end catch
#ifdef DEBUG
                cerr << "at 3.4.1: d="<<d<<", idx="<<idx<<endl;
#endif
                // succeeded in parsing a number, keep track of it:
                count_this_line++;
                im.data.push_back(d); // store into Image private data
                
                // get the next piece of the input line:
                input_piece = input_piece.substr(idx);
            }//end while: input_piece not empty
            
#ifdef DEBUG
            cerr << "at 3.5"<<endl;
#endif
            
            //at this point, the line is parsed into a bunch of numbers.
            //remember how many if this is the first line:
            if(input_line_number==0) {
                im.nx = count_this_line;// store into Image private data
            } else {
                // signal error if count of numbers is different:
                if(im.nx != count_this_line) {
                    cerr << "Error in Image reader: nx not consistent for line #"<<input_line_number<<endl;
                    throw bad_exception();
                }//endif
            }// endif
            
#ifdef DEBUG
            cerr << "at 3.6, nx="<<im.nx<<endl;
#endif
            
            // get the next input line:
            try{
	      getline(in, input_line);
	    }//end-try
	    catch(...) {
	    }//end-catch
            input_line_number++;
            
#ifdef DEBUG    
            cerr << "at 3.7, input_line_number="<<input_line_number<<endl;
#endif
            
        }// end while: read succeeded
        
        // set the number of lines:
        im.ny = input_line_number;// store into Image private data
        
#ifdef DEBUG
        cerr << "at 3.8, ny="<<im.ny<<endl;
#endif

        return in;
        
        
    }//end Image()
    







//-----------------------------------------------------------------
// function filter:
// input parameters:  filter_name: a string with one of these values:
//                                 "threshhold"
//                                 any other value is an error
//                    param: value of the parameter needed to run the
//                           filter specified by filter_name.
//                           for filter="threshhold", param=the threshhold
// returns: nothing
// summary: filters the image in place using the algorithm specified.
//          throws bad_exception if:
//               nx or ny are zero
//               size() of image is not equal to nx*ny
//          throws invalid_argument if:
//               unknown filter_name
//-----------------------------------------------------------------
    void Image::filter(string filter_name, double param) 
    {
        
        // error-check:
        if(nx<=0){
            cerr<<"Error in filter: image x-size is <= zero."<<endl;
            throw bad_exception();
        }//endif
        if(ny<=0){
            cerr<<"Error in filter: image y-size is <=zero."<<endl;
            throw bad_exception();
        }//endif
        if(data.size() != nx*ny) {
            cerr<<"Error in filter: image-size is not nx*ny."<<endl;
            throw bad_exception();
        }//endif
        
        // perform the filter operation
        if(filter_name.compare("threshhold")==0){
            
            double thresh = param; // for readability


            cout << "thresh = " << thresh << endl; //debug



            for(int i=0;i<nx*ny;i++){
              cout << data[i] << endl;//debug
              if(data[i] <= thresh) {
                  data[i]=0.0;
                  cout <<"setting to 0" << endl;//debug
              }// endif
            }//endfor
            
            //add more comparisons with other strings to implement other filters here.....
            
        }
        else {
            cerr<<"Error in filter: unknown filter name: "<<filter_name<<endl;
            throw invalid_argument("");
        }//endif

    }// end filter()
    


//-----------------------------------------------------------------
// overloaded operator <<:
// input parameters:  out: an already open ostream
//                    im:  an image to write out
// summary: writes double ascii data to output stream
//          throws bad_exception if:
//               nx or ny are zero
//               size() of image is not equal to nx*ny
//-----------------------------------------------------------------    
    ostream& operator<< (ostream &out, Image &im)
    {
        
        
        // other checks
        if(im.nx<=0){
            cerr<<"Error in Image output: image x-size is <=zero."<<endl;
            throw bad_exception();
        }//endif
        if(im.ny<=0){
            cerr<<"Error in Image output: image y-size is <=zero."<<endl;
            throw bad_exception();
        }//endif
        if(im.data.size() != im.nx*im.ny) {
            cerr<<"Error in Image output: image-size is not nx*ny."<<endl;
            throw bad_exception();
        }//endif
        
        
        // write out: nx doubles per line:
        for(int j=0;j<im.ny;j++){
            for(int i=0;i<im.nx;i++){
                out << im.data[ j*im.nx + i ]<<" ";          
            }//endfor: i
            out << endl;
        }//endfor: j
        
        
        return out;
    }// end: operator<<



//-----------------------------------------------------------------
    // read a single-channel image file
    // infile is the name of the intput image file
    // if the image is more than 1 channel, it just uses the first.
    // for now, it creates a float channel.
    // returns the newly-created Image
//-----------------------------------------------------------------
    void Image::read_file(const std::string &infile){
    // 1. open the data file
    // 2. figure out data characteristics
    // 3. create empty raster
    // 4. fill raster.
    FileOpenErrorException FileOpenError;

#ifdef DEBUG
    cout << "read_file: at 1"<<endl;
#endif
    // 1. open the data file
    GDALDataset *poDataset;
    GDALAllRegister();
    char cname[infile.length()+10]; // need a copy because it's declared const
    strcpy(cname, infile.c_str());
    poDataset = (GDALDataset *) GDALOpen(cname, GA_ReadOnly );
    if( poDataset == NULL ) throw FileOpenError;

#ifdef DEBUG
    cout << "read_file: at 2"<<endl;
#endif
    // 2. figure out data characteristics
    GDALRasterBand *poBand;
    poBand = poDataset->GetRasterBand( 1 ); // the first channel.
    int nx = poBand->GetXSize();
    int ny = poBand->GetYSize();

#ifdef DEBUG
    cout << "read_file: at 3"<<endl;
#endif
    // 3. create empty raster (for efficiency)
    data.resize(nx*ny);

#ifdef DEBUG
    cout << "read_file: at 4"<<endl;
#endif
    // 4. temporary place for one line of the image:
    float *buf;
    buf = (float *) CPLMalloc(sizeof(float)*nx);

#ifdef DEBUG
    cout << "read_file: at 5"<<endl;
#endif
    // 5. fill the data vector:
    int pix=0;
    for(int iy=0;iy<ny;++iy){
      CPLErr err = poBand->RasterIO( GF_Read, 
                    0, iy, nx, 1,
                    buf, nx, 1, GDT_Float32,
                    0, 0 );
      for(int ix=0;ix<nx;++ix) {
        data[pix++]=buf[ix];
      }//endfor: ix

    } // endfor: iy

#ifdef DEBUG
    cout << "read_file: at 6"<<endl;
#endif
    CPLFree(buf);
    GDALClose(poDataset);

#ifdef DEBUG
    cout << "read_file: at 7"<<endl;
#endif
    // 6. finish specifiying the new image:
    this->nx = nx;
    this->ny = ny;

    }// end: Image::read_file


//-----------------------------------------------------------------
// Write out the image data as a 1-channel jpeg.
//-----------------------------------------------------------------
    void Image::write_file(const std::string &outfile){
    
      const char *pszFormat = "GTiff";
      GDALDriver *poDriver;
      char **papszMetadata;
      poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
      if( poDriver == NULL ) throw std::runtime_error("invalid GDAL driver");

      papszMetadata = poDriver->GetMetadata();

      //temporary:
      //if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATE, FALSE ) )
      //  printf( "Driver %s supports Create() method.\n", pszFormat );
      //if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATECOPY, FALSE ) )
      //  printf( "Driver %s supports CreateCopy() method.\n", pszFormat );


      // some fake, stupid stuff for now:
      GDALDataset *poDstDS;
      char **papszOptions = NULL;
      poDstDS = poDriver->Create( outfile.c_str(), nx, ny, 1, GDT_Byte,
                                  papszOptions );
      
      double adfGeoTransform[6] = { 444720, 30, 0, 3751320, 0, -30 };
      OGRSpatialReference oSRS;
      char *pszSRS_WKT = NULL;
      GDALRasterBand *poBand;

      poDstDS->SetGeoTransform( adfGeoTransform );
      oSRS.SetUTM( 11, TRUE );
      oSRS.SetWellKnownGeogCS( "NAD27" );
      oSRS.exportToWkt( &pszSRS_WKT );
      poDstDS->SetProjection( pszSRS_WKT );
      CPLFree( pszSRS_WKT );

      poBand = poDstDS->GetRasterBand(1);
      //poBand->RasterIO( GF_Write, 0, 0, 512, 512,
      //                  abyRaster, 512, 512, GDT_Byte, 0, 0 );


    // 4. temporary place for one line of the image:
      GByte buf[nx*ny];


    // 5. fill the output:
    int pix=0;
    int dd;
    for(int iy=0;iy<ny;++iy){
      
      for(int ix=0;ix<nx;++ix) {

        //force output to be a single byte per pixel:
        dd= data[iy*nx+ix];
        if(dd<0) {
          buf[ix]=0;
        } else if(dd>255) {
          buf[ix]=255;
        } else {
          buf[ix]=dd;
        }//endif

      }//endfor: ix

      CPLErr err = poBand->RasterIO( GF_Write, 
                    0, iy, nx, 1,
                    buf, nx, 1, GDT_Byte,
                    0, 0 );

    } // endfor: iy



      /* Once we're done, properly close the dataset */
      GDALClose( (GDALDatasetH) poDstDS );


    }// end: Image::write_file
