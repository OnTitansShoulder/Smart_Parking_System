#ifndef WINDOWS
#define WINDOWS
#include <SDL.h>
#include "Graphics_Texture.h"

class LWindow
{
public:
	//Intializes internals
	LWindow();

	//Creates window
	bool init(int& i);

	//Handles window events
	void handleEvent(SDL_Event& e);

	//Focuses on window
	void focus();

	//Shows windows contents
	void render();
	void render(LTexture gtexture);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();
	SDL_Renderer* getRenderer();

	//Window focii
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();
	bool isShown();

private:
	//Window data
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	int mWindowID;

	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;
	bool mShown;
};

#endif