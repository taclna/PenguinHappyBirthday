#pragma once
#include "Windows.h"
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Background.h"
#include <iostream>
#include <sstream>
#include "Penguin.h"
class customTime
{
public:
	customTime(SDL_Renderer* renderer, TTF_Font* font ,Penguin* penguin);
	~customTime();
	void update();
	void render();
	void setTexture();
	std::string SystemTimeToString(const SYSTEMTIME& st);


private:
	SDL_Renderer* renderer;
	SYSTEMTIME st;
	std::string time;
	TTF_Font* font;
	Background* background;
	SDL_Texture* texture;
	Penguin* penguin;
	int textWidth, textHeight;
	bool isRender;


};

