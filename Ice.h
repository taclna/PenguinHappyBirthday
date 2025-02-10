#pragma once
#include <vector>
#include <SDL.h>
#include <string>
#include <iostream>
#include <SDL_image.h>
class Ice
{
public:
	Ice(SDL_Renderer* renderer, const std::vector<std::string>& iceImgs);
	~Ice();
	/*void update();
	void render();*/
private:
	SDL_Texture* loadTexture(const std::string& path);

	SDL_Renderer* renderer;
	int hitCount;
	std::vector<SDL_Texture*> iceTextures;

};

