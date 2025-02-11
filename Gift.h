#ifndef GIFT_H
#define GIFT_H

#include <SDL.h>
#include "Present.h"

class Gift {
public:
    Gift(SDL_Renderer* renderer, const char* texturePath, Present* present, int x, int y, int h);  // Constructor sửa lại
    ~Gift();

    void render(SDL_Renderer* renderer);
    void handleEvent(SDL_Event& e);
    void update();
    bool isMouseInside();

private:
    SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* texturePath);
    void renderRoundedRect(SDL_Renderer* renderer, SDL_Rect rect, int radius);

    SDL_Rect rect;
    SDL_Texture* texture;
    Present* present;
    int originalWidth, originalHeight;
    bool isClicked;
    bool isVisible;
    bool isBig;
};

#endif
