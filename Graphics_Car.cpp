#include "Graphics_Car.h"

SDL_mutex* Car::gBufferLock = SDL_CreateMutex();
std::vector<Car*> Car::carDeck = {};

Car::Car(SDL_Renderer* gRenderer, LTexture& texture, LTexture& flrTexture, int initX, int initY, Garage* garage)
{
	//Initialize the position
	m_PosX = initX;
	m_PosY = initY;
	g = garage;
	
	CarRenderer = gRenderer;
	g_CarTexture = texture;
	g_FloorTexture = flrTexture;
}

Car::~Car()
{
	remove();
}

void Car::exit(int a, int b)
{
	g->floorStatus[a][b] = 0;
	//Lock
	//SDL_LockMutex(gBufferLock);
	remove();

	//Unlock
	//SDL_UnlockMutex(gBufferLock);
}



void Car::remove()
{
	renderOut();
	m_PosX = -40;
	m_PosY = -40;
}
 
//Rendered again, does it eliminate original texture?
void Car::render()
{
	//Render Car's new position
	//printf("\nRending car at %d %d", m_PosX, m_PosY);
	g_CarTexture.render((int)m_PosX, (int)m_PosY, CarRenderer);
	SDL_RenderPresent(CarRenderer);
}

//Use background clip to over render the top of the car texture
void Car::renderOut()
{
	SDL_Rect clip = {m_PosX, m_PosY, 40, 40};
	//printf("\nx: %4f y: %4f", m_PosX,  m_PosY);
	g_FloorTexture.render((int)m_PosX, (int)m_PosY, CarRenderer, &clip);
	
	SDL_RenderPresent(CarRenderer);
}