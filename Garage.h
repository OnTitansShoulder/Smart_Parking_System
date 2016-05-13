#ifndef GARAGE_H
#define GARAGE_H

#include "Build.h"

class Garage{
private:

public:
	//To store and keep track of floor parking lot status
	int** floorStatus; 
	
    //garage constructor
	Garage();

    bool checkUFID(const std::string ufid);
    int matchDecalType(const int ufid);
	int findFirst(const int id);
};
#endif