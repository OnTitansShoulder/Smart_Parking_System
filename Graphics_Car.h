#ifndef CAR_H
#define CAR_H
#include "Graphics_Texture.h"
#include "Garage.h"

//This Car object is mostly responsible for rending the car texture onto the windows
class Car 
{
private:
	static SDL_mutex *gBufferLock;
	SDL_Renderer* CarRenderer;
	LTexture g_CarTexture;
	LTexture g_FloorTexture;
	int m_PosX, m_PosY;
	Garage* g;
public:
	static std::vector<Car*> carDeck;

	//Car texture dimension specifications
	static const int CAR_WIDTH = 20;
	static const int CAR_HEIGHT = 20;

	Car(SDL_Renderer* gRenderer, LTexture& texture, LTexture& flrTexture, int initX, int initY, Garage* garage);

	~Car();

	void remove();

	void exit(int a, int b);

	void render();
	 
	void renderOut();
};

#endif