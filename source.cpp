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
    vector<Raster> layers = img.get_layers();
    for(int i = 0; i < layers.size(); i++){
        
    }

    return 0;
}