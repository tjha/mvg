//=========================================================
// Source.cpp
// By Tejas Jha, April 9 2018
//
// testing of functionalities of created classes, simulating
// how a user may use the functions
//=========================================================

#include <iostream>
#include <vector>
#include "Classes.h"

using namespace std;

int main(int argc, char* argv[]){
    
    Image img("cropped.jpg", 3);
    

/*

    vector<Slice> train;
    Slice slc = { 200, 300, 700, 700, 1};
    train.push_back(slc);
    img.set_training(train);
    vector<Raster> layers = img.get_layers();
    cout << layers.size() << endl;

    Raster rast = layers[3];
    vector<int> vec = rast.get_data();
*/

    /*
    for(int x = 0; x < rast.xsize(); x++){
        for(int y = 0; y < rast.ysize(); y++){
            cout << vec[y*rast.xsize() + x] << ' ';
        }
        cout << endl;
    }
    */

    /*
    for(int i = 0; i < layers.size(); i++){
        vector<int> vec = layers[i].get_data();
        for(int x = 0; x < layers[i].xsize(); x++){
            for(int y = 0; y < layers[i].ysize(); y++){
                cout << vec[y*layers[i].xsize() + x] << ' ';
            }
            cout << endl;
        }
    }
    */

    return 0;
}