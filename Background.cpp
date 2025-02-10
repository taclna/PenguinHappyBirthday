#include "Background.h"
#include <cmath>
#include <iostream>

Background::Background(SDL_Renderer* renderer, const std::string& imagePath, SDL_Rect screen)
    : screenSize(screen), transitionRadius(0), transitioning(false), toBlack(false), currentState(Penguin::CANDLE_OFF), imageShown(false) {
    // Load texture của ảnh nền
    SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());
    if (!imageSurface) {
        std::cerr << "Unable to load image: " << imagePath << " " << IMG_GetError() << std::endl;
        imageTexture = nullptr;
    }
    else {
        imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
    }
}

Background::~Background() {
    if (imageTexture) SDL_DestroyTexture(imageTexture);
}

void Background::update(Penguin::State newState) {
    if (newState != currentState) {
        currentState = newState;
    }
}

void Background::render(SDL_Renderer* renderer) {
    if (currentState == Penguin::CANDLE_OFF && !imageShown) {
        // Hiển thị nền đen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
    else {
        // Hiển thị ảnh nền
        imageShown = true;
        if (imageTexture) {
            SDL_RenderCopy(renderer, imageTexture, nullptr, nullptr);
        }
    }
}

SDL_Texture* Background::loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        std::cerr << "Unable to load image " << path << ": " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (!texture) {
        std::cerr << "Unable to create texture from " << path << ": " << SDL_GetError() << std::endl;
    }

    return texture;
}

bool Background::checkImgShown() {
    return imageShown;
}
