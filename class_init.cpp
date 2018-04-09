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
	
#include "Image.h"

// Below #includes may need adjustments
#include "Exceptions.hpp"
#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()
#include "cpl_string.h"
#include <ogr_spatialref.h>