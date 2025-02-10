#ifndef PRESENT_H
#define PRESENT_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include <iostream>
#include "MessageDisplay.h"
#include "Penguin.h"

class Present {
public:
    Present(SDL_Renderer* renderer, const std::vector<std::string>& imagePaths, const std::string& soundPath, SDL_Rect position, MessageDisplay* message, Penguin* penguin);
    ~Present();

    void handleEvent(const SDL_Event& e);
    void update();
    void render();
    void playSound();
    bool isRunning() const;

    bool isMouseInside(int x, int y);
    bool isRendering();

private:
    SDL_Renderer* renderer;
    std::vector<SDL_Texture*> textures;
    Mix_Chunk* soundEffect;

    int currentImageIndex;
    bool isAnimating;
    Uint32 lastUpdateTime;

    SDL_Rect position;

    MessageDisplay* message;
    Penguin* penguin;

    bool isRender;

    SDL_Texture* loadTexture(const std::string& path);
};

#endif
