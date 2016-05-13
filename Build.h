#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include <ctime>

//Screen dimension constants and Total windows
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int TOTAL_WINDOWS = 4;
const int TOTAL_LOTS_PER_ROW = 14;
const int MAXSLOT = 56;
const int FLOORPIXEL[] = {420, 300, 140, 20};

enum FloorPlans
{
	FIRST_FLOOR = 0,
	SECOND_FLOOR = 1,
	THIRD_FLOOR = 2,
	FOURTH_FLOOR = 3,
	TOTAL_FLOORS = 4
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Helper functions: random number generators
int randomID();

int randomTime();

int randomNum(int gap);

Uint32 callback(Uint32 interval, void* param);

int threadOne(void* g);
int threadTwo(void* g);
int threadThree(void* g);
int threadFour(void* g);

