#pragma once
//Using SDL, SDL Threads, SDL_image, standard IO, and, strings
#include <SDL_thread.h>
#include <sstream>
#include "Graphics_Car.h"
#include "Graphics_Windows.h"
#include "Garage.h"
#include "LinkedList.h"
#include "Build.h"

bool QUIT = false;
bool USERACTIVE = false;
//The window we'll be rendering to
LWindow* gWindows[TOTAL_WINDOWS]; 

//Scene textures
LTexture gBackgroundTexture[TOTAL_FLOORS]; //To initialize windows' backgrounds
LTexture gBackgroundRerender[TOTAL_FLOORS]; //To be used to cover up car textures
LTexture gCarTexture[TOTAL_WINDOWS]; //Car textures for randomly generated cars
LTexture gCarTextureUser[TOTAL_WINDOWS]; //Car textures for user entered cars


LinkedList* userList; //Store and keep track of users' cars information
//Mutex

//The protective mutex
SDL_mutex* garageBufferLock = SDL_CreateMutex();

 //Initialize the windows and renderer utilities from SDL2
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		for (int j = 0; j < TOTAL_WINDOWS; j++)
			gWindows[j] = new LWindow();
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		int i = 0;
		//Create windows
		if (!gWindows[i]->init(i))
		{
			printf("Window 0 could not be created!\n");
			success = false;
		}
		if (success)
		{
			//Initialize the rest of the windows
			for (i = 1; i < TOTAL_WINDOWS; ++i)
			{
				gWindows[i]->init(i);
			}
		}
	}

	return success;
}

//To load textures from files
bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load floor plan textures
	if (!gBackgroundTexture[0].loadFromFile("1stFloor_Resources.png", gWindows[0]->getRenderer()))
	{
		printf("Failed to load Floor Plan texture!\n");
		success = false;
	}
	if (!gBackgroundTexture[1].loadFromFile("2ndFloor_Resources.png", gWindows[1]->getRenderer()))
	{
		printf("Failed to load Floor Plan texture!\n");
		success = false;
	}
	if (!gBackgroundTexture[2].loadFromFile("3rdFloor_Resources.png", gWindows[2]->getRenderer()))
	{
		printf("Failed to load Floor Plan texture!\n");
		success = false;
	}
	if (!gBackgroundTexture[3].loadFromFile("4thFloor_Resources.png", gWindows[3]->getRenderer()))
	{
		printf("Failed to load Floor Plan texture!\n");
		success = false;
	}
	else
	{
		//loading background textures for rerender over the cars
		gBackgroundRerender[0].loadFromFile("1stFloor_Resources.png", gWindows[0]->getRenderer());
		gBackgroundRerender[1].loadFromFile("2ndFloor_Resources.png", gWindows[1]->getRenderer());
		gBackgroundRerender[2].loadFromFile("3rdFloor_Resources.png", gWindows[2]->getRenderer());
		gBackgroundRerender[3].loadFromFile("4thFloor_Resources.png", gWindows[3]->getRenderer());
	}

	if (!gCarTexture[0].loadFromFile("carTexture.png", gWindows[0]->getRenderer()))
	{
		printf("Failed to load car texture!\n");
		success = false;
	}
	else
	{
		for (int i = 1; i < TOTAL_WINDOWS; i++)
		{
			gCarTexture[i].loadFromFile("carTexture.png", gWindows[i]->getRenderer());
		}
		for (int i = 0; i < TOTAL_WINDOWS; i++)
			gCarTextureUser[i].loadFromFile("carTextureUser.png", gWindows[i]->getRenderer());
	}
	return success;
}

//Deallocate memory, free memory and destroy objects
void close()
{
	//Free loaded images
	for (LTexture t : gCarTexture)
		t.free();

	//Mutex
	SDL_DestroyMutex(garageBufferLock);
		
	for (int i = 0; i < TOTAL_FLOORS; i++)
	{
		//Free loaded images
		gBackgroundTexture[i].free();
		gCarTexture[i].free();
		gCarTextureUser[i].free();
		//Destroy window
		gWindows[i]->free();
	}

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


//Thread One: To initialize cars on the floor
int threadOne( void* g)
{
	printf("\nGarage initialization...");
	//Here i is tracking the floor number, j is tracking the 
	//Parking lot row number, and k is tracking the parking lot number on each row
	
	int i, j, k, x = -40, y = -40, a, b, random;
	SDL_Renderer* renderer = 0;
	Garage* garage = static_cast<Garage*>(g);

	for (i = 0; i < TOTAL_FLOORS; i++) // Floor
	{
		for (k = 0; k < TOTAL_FLOORS; k++) // Parking Lot Row
		{
			for (j = 1; j <= TOTAL_LOTS_PER_ROW; j += random) // Parking Lot Number
			{
				renderer = gWindows[i]->getRenderer();

				a = i;
				b = j + k * 14 - 1;
				garage->floorStatus[a][b] = -1;

				//Process the x, y positions to pixel positions
				x = j * 40;
				y = FLOORPIXEL[k];
				Car* temp = new Car(renderer, gCarTexture[i], gBackgroundRerender[i], x, y, garage);
				temp->render();

				//Set callback
				//SDL_AddTimer(time * 1000, callback, temp);
				random = randomNum(2);
			}
		}
	}
	printf("complete.\n");
	return 0;
}

//Thread Two: Randomly generate Cars. Simulate the car flow of the Garage
int threadTwo(void* g)
{
	SDL_Delay(6000);
	printf("\nSmart Parking System starts operating.\n");
	//Here x, y are positions for cars to be rendered on Windows
	//Cast the void pointer into Garage pointer type
	int x = -40, y = -40, fl = 0;
	SDL_Renderer* render = 0;
	Garage* garage = static_cast<Garage*>(g);
	for (; !QUIT; SDL_Delay(randomNum(5) * 1000))
	{
		if (!USERACTIVE)
		{
			//here x is set as floor number and y is set as parking lot number
			fl = randomNum(4);
			render = gWindows[fl - 1]->getRenderer();

			//Now get the position for rendering
			int a = garage->findFirst(-1 * fl) + 1;
			if (a == 0)
				continue;
			x = (a % 14 == 0 ? 14 : a % 14) * 40;
			y = FLOORPIXEL[(a % 14 == 0 ? a / 14 - 1 : a / 14)];

			Car* temp = new Car(render, gCarTexture[fl - 1], gBackgroundRerender[fl - 1], x, y, garage);
			temp->render();

			//printf("\nNew Car entered at fl %d and parkinglot %d", fl, a);
		}
	}
	return 0;
}

//Thread Three: User input ultimate Thread!!!
int threadThree(void* g)
{
	//Here x, y are positions for cars to be rendered on Windows; a, b are 
	//Cast the void pointer into Garage pointer type
	int i, j, k, x = -40, y = -40, a, b, random, ufid;
	SDL_Renderer* renderer = 0;
	Garage* garage = static_cast<Garage*>(g);

	while (!QUIT)
	{
		int choice = -1;
		bool menu = true;
		int UFID;

		do
		{
			choice = -1;
			std::cout << " \n Please select one of the following options:  \n"
				<< "1 - Entering \n2 - Leaving \n3 - Exit" << std::endl
				<< "Enter your selection: " << std::endl;
			std::cin >> choice;

			switch (choice)
			{
				case 1: //Process user's inputs for enter new Car
				{
						USERACTIVE = true;
					  int flag1;
					  std::cout << "\nYou are now entering the garage."
						<< "\nPlease enter your UF ID: ";

					  do{
						 flag1 = 0;
						 std::string id;
						 std::cin >> id;
						 if (!garage->checkUFID(id)) {
							 std::cout << "\nInvalid input, please enter your UFID again.\n" << std::endl;
							 flag1 = 1;
						}
						else {
							ufid = atoi(id.c_str());

							Node* check = userList->find(ufid);
							if (check != NULL)
							{
								printf("\nThis UFID has been used to park somewhere in this garage!\n");
								flag1 = 1;
								break;
							}
							a = garage->matchDecalType(ufid);
							b = garage->findFirst(ufid) + 1;
							renderer = gWindows[a - 1]->getRenderer();
							//break the loop the user has parked in this g or there is no available space
							if (b == 0) {
								std::cout << "\nThe garage is currently full. Failed to assign a parking lot."
									<< " Please come and check again later. \n";
								flag1 = 1;
							}
							else{
								SDL_LockMutex(garageBufferLock);
								x = (b % 14 == 0 ? 14 : b % 14) * 40;
								y = FLOORPIXEL[(b % 14 == 0 ? b / 14 - 1 : b / 14)];

								Car* temp = new Car(renderer, gCarTextureUser[a - 1], gBackgroundRerender[a - 1], x, y, garage);
								Node* user = new Node(ufid, a - 1, b - 1, temp);
								userList->add(user);
								temp->render();
								SDL_UnlockMutex(garageBufferLock);
								//print out the assigned Floor and slot for the user
								std::cout << "\nYou are assigned a slot at:\n\nFloor number: " << a <<
									"\nSlot number: " << b << "\n\n";
							}
						}
					} while (flag1 == 1);
					  USERACTIVE = false;
					 break;
				}
							
				case 2: //Process user's inputs for leaving the garage
				{
						  USERACTIVE = true;
						  int flag2;
					std::cout << "\nYou are now leaving the garage."
						<< "\nPlease enter your UF ID: ";
					

					do{
						flag2 = 0;
						std::string id;
						std::cin >> id;
						if (!garage->checkUFID(id)) {
							std::cout << "\nInvalid input, please enter your UFID again.\n" << std::endl;
							flag2 = 1;
						}
						else {
							ufid = atoi(id.c_str());
							userList->removeUser(ufid);
						}
					} while (flag2 == 1);
					
					USERACTIVE = false;
					break;
				}
				case 3:
				{
					QUIT = true;
					break;
				}
				default:
				{
					std::cout << "\nInvalid Selection. Please enter valid input.";
					std::cin.clear();
					std::cin.ignore(10000, '\n');
					USERACTIVE = false;
					break;
				}
			}

		} while ((choice == 1) || (choice == 2));
	}
	printf("\nExiting the user interface...Complete");
	return 0;
}

//Remove random generated cars randomly
int threadFour(void* g)
{
	int x = -40, y = -40, fl = 0;
	SDL_Renderer* render = 0;
	Garage* garage = static_cast<Garage*>(g);
	for (; !QUIT; SDL_Delay(randomNum(6) * 1000))
	{
		if (!USERACTIVE)
		{
			//here x is set as floor number and y is set as parking lot number
			fl = randomNum(4);
			render = gWindows[fl - 1]->getRenderer();

			//Now get the position for rendering
			int a = randomNum(56);
			if (garage->floorStatus[fl-1][a-1] == 1)
				continue;
			x = (a % 14 == 0 ? 14 : a % 14) * 40;
			y = FLOORPIXEL[(a % 14 == 0 ? a / 14 - 1 : a / 14)];

			Car* temp = new Car(render, gCarTexture[fl - 1], gBackgroundRerender[fl - 1], x, y, garage);
			temp->exit(fl-1, a-1);

			//printf("\nNew Car entered at fl %d and parkinglot %d", fl, a);
		}
	}
	return 0;
}

//Helper function to generate a random number
int randomNum(int gap)
{
	return (rand() % (gap) + 1);
}

//main method
int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Event handler
			SDL_Event e;

			//Set random seed
			srand(time(NULL));
			
			//Render all windows with initial background texture
			for (int i = 0; i < TOTAL_FLOORS; i ++)
				gWindows[i]->render(gBackgroundTexture[i]);
			
			//declare and initialize data resources that will be shared to many threads
			Garage* garage = new Garage();
			userList = new LinkedList();

			
			//Thread One: To initialize cars on the floor
			SDL_Thread* ThreadOne = SDL_CreateThread(threadOne, "ThreadOne", garage);
			SDL_WaitThread(ThreadOne, NULL);
			

			//Thread Two: Randomly produce Cars into the Graphics
			SDL_Thread* ThreadTwo = SDL_CreateThread(threadTwo, "ThreadTwo", garage);

			//Thread Three: Obtain user's input for Cars into Garage
			SDL_Thread* ThreadThree = SDL_CreateThread(threadThree, "ThreadThree", garage);
			
			//Thread Three: remove the random generated cars randomly
			SDL_Thread* ThreadFour = SDL_CreateThread(threadFour, "ThreadFour", garage);

			//Purely checking whether the program is still running 
			while (!QUIT)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						QUIT = true;
					}

					for (int i = 0; i < TOTAL_WINDOWS; ++i)
					{
						gWindows[i]->handleEvent(e);
					}
				}


				//Check all windows
				bool allWindowsClosed = true;
				for (int i = 0; i < TOTAL_WINDOWS; ++i)
				{
					if (gWindows[i]->isShown())
					{
						allWindowsClosed = false;
						break;
					}
				}

				if (allWindowsClosed)
					QUIT = true;

			}

		
		}
	}
	printf("\nProgram will be shut down in five seconds. ");
	SDL_Delay(5000);
	//Free resources and close SDL
	close();

	return 0;
}