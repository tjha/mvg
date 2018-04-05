GDAL_INCLUDES=-I/home/lep/MDP2/codes/demo10_complete/gdal-2.1.3/include
GDAL_LIBRARIES=/home/lep/MDP2/codes/demo10_complete/gdal-2.1.3/lib/libgdal.a \
	        -lfreexl -L/usr/local/lib -lgeos_c  -lsqlite3 -lodbc -lodbcinst \
		-lexpat -lxerces-c -lpthread -ljasper -lnetcdf \
		-L/home/lep/MDP2/codes/demo9/hdf5-1.10.0-patch1/lib \
		-lhdf5  -logdi -lgif -ljpeg -lpng -lcfitsio -L/usr/lib -lpq \
		-lz -lpthread -lm -lrt -ldl -lcurl -lxml2


main: main.cpp Image.o
	g++ -g -std=c++0x -o main main.cpp Image.o ${GDAL_INCLUDES} ${GDAL_LIBRARIES}

Image.o: Image.cpp Image.h Exceptions.hpp
	g++ -c -g -std=c++0x -o Image.o Image.cpp  ${GDAL_INCLUDES}
