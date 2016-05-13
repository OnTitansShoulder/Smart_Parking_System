#include "Garage.h"

//garage constructor
Garage::Garage() 
{
	floorStatus = new int*[TOTAL_FLOORS];
	for (int i = 0; i < TOTAL_FLOORS; i++) {
		floorStatus[i] = new int[MAXSLOT];
		for (int j = 0; j < MAXSLOT; j++)
			floorStatus[i][j] = 0;
	}
}

//check if the user input is an 8 digit string
bool Garage::checkUFID(const std::string ufid) {
    if(ufid.length() != 8 || ufid.at(0) == '0') {
        return false;
    }
    for(auto it = ufid.cbegin(); it != ufid.cend(); it++) {
        if(!isdigit(*it)) {
            return false;
        }
    }
    return true;
}

// get the corresponding floorNum that represents the decal type
// return a number from 1 to 4
int Garage::matchDecalType(const int ufid) {
    int floorNum;
    int firstDigit = ufid/10000000;
    switch (firstDigit){
        case 1:
        case 2:
            floorNum = 1;
            break;
        case 3:
        case 4:
            floorNum = 2;
            break;
        case 5:
        case 6:
            floorNum = 3;
            break;
        case 7:
        case 8:
        case 9:
            floorNum = 4;
            break;
    }
    return floorNum;
}

//find the first free space in the corresponding floor and return the index of the slot numbered betwee 0 and 55
int Garage::findFirst(const int id)
{
	//find free slot for random cars
	if (id < 0 && id >= -4)
	{
		for (int i = 0; i < MAXSLOT; i++)
		{
			if ((int)floorStatus[-1 * id - 1][i] == 0)
			{
				floorStatus[-1 * id - 1][i] = -1;
				return i;
			}
		}
	}
	else
	{
		//find free slot for real user with an UFID
		int floor = matchDecalType(id) - 1;
		
		for (int i = 0; i < MAXSLOT; i++)
		{
			if (floorStatus[floor][i] == 0)
			{
				floorStatus[floor][i] = 1;
				return i;
			}
		}
	}
	return -1;
}