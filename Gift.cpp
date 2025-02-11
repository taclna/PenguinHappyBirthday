#include "Gift.h"
#include <iostream>
#include <SDL_image.h>

Gift::Gift(SDL_Renderer* renderer, const char* texturePath, Present* present ,int x, int y, int h) 
    : present(present), isVisible(false), isClicked(false), isBig(false) {
    rect.x = x;
    rect.y = y;
    rect.h = h;
    texture = loadTexture(renderer, texturePath);

    
    float aspectRatio = static_cast<float>(originalWidth) / static_cast<float>(originalHeight);
    rect.w = static_cast<int>(h * aspectRatio); 
}

Gift::~Gift() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

SDL_Texture* Gift::loadTexture(SDL_Renderer* renderer, const char* texturePath) {
    SDL_Surface* surface = IMG_Load(texturePath);
    if (!surface) {
        std::cerr << "Error loading image: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    originalWidth = surface->w;
    originalHeight = surface->h;

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return newTexture;
}

void Gift::render(SDL_Renderer* renderer) {
    if (isVisible) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        renderRoundedRect(renderer, rect, 20);
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

void Gift::renderRoundedRect(SDL_Renderer* renderer, SDL_Rect rect, int radius) {
    /*for (int x = rect.x; x < rect.x + rect.w; x++) {
        for (int y = rect.y; y < rect.y + rect.h; y++) {
            if ((x - rect.x) < radius && (y - rect.y) < radius) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }*/
}

void Gift::handleEvent(SDL_Event& e) {
    if (!isVisible) return;
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (isMouseInside()) {
            if (!isClicked) {
                isClicked = true;
                isBig = true;
                rect.w *= 5;
                rect.h *= 5;
                rect.x = (800 - rect.w) / 2;
                rect.y = (600 - rect.h) / 2;
            }
            else if (isBig) {
                isVisible = false;
            }
        }
    }
}

void Gift::update() {
    if (present->checkDone() && !isBig) {
        isVisible = true;
    }
}

bool Gift::isMouseInside() {
    if (!isVisible) return false;
    int x, y;
    SDL_GetMouseState(&x, &y);
    return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
}
