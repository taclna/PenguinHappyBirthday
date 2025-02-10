#include "Penguin.h"
#include <iostream>

Penguin::Penguin(SDL_Renderer* renderer, MessageDisplay* message,const std::vector<std::string>& candleOffImgs,
    const std::vector<std::string>& candleOnImgs, const std::vector<std::string>& blowCandleImgs,
    const std::vector<std::string>& showCandleImgs)
    : currentState(CANDLE_OFF), animationFrame(0), lastFrameTime(0), frameDuration(100), message(message) {
    for (const auto& img : candleOffImgs) {
        candleOffTextures.push_back(loadTexture(img, renderer));
    }
    for (const auto& img : candleOnImgs) {
        candleOnTextures.push_back(loadTexture(img, renderer));
    }
    for (const auto& img : blowCandleImgs) {
        blowCandleTextures.push_back(loadTexture(img, renderer));
    }
    for (const auto& img : showCandleImgs) {
        showCandleTextures.push_back(loadTexture(img, renderer));
    }

    if (!candleOffTextures.empty()) {
        int w, h;
        SDL_QueryTexture(candleOffTextures[0], nullptr, nullptr, &w, &h);
        position = { (screenWidth - w) / 2, (screenHeight - h) / 2 + 190, w, h};
    }
    else {
        position = { 100, 100, 64, 128 }; // Kích thước mặc định
    }
}

Penguin::~Penguin() {
    for (auto texture : candleOffTextures) SDL_DestroyTexture(texture);
    for (auto texture : candleOnTextures) SDL_DestroyTexture(texture);
    for (auto texture : blowCandleTextures) SDL_DestroyTexture(texture);
    for (auto texture : showCandleTextures) SDL_DestroyTexture(texture);
}

SDL_Texture* Penguin::loadTexture(const std::string& path, SDL_Renderer* renderer) {
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


void Penguin::switchState(State newState) {
    currentState = newState;
    animationFrame = 0;
    lastFrameTime = SDL_GetTicks();
}

void Penguin::setVolume(float volume) {
    currentVolume = volume;
    std::cout << volume << '\n';
    if (currentState == CANDLE_ON && currentVolume > 50.0f && message->isFinished()) {
        switchState(BLOW_CANDLE); // Chuyển sang trạng thái BLOW_CANDLE
    }
}

void Penguin::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (x >= position.x && x <= position.x + position.w && y >= position.y && y <= position.y + position.h) {
            if (currentState == CANDLE_OFF) {
                switchState(SHOW_CANDLE);
            }
            else if (currentState == CANDLE_ON) {
                switchState(BLOW_CANDLE);
            }
        }
    }
}

void Penguin::update() {
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - lastFrameTime > frameDuration) {
        animationFrame++;
        lastFrameTime = currentTime;

        const std::vector<SDL_Texture*>* currentTextures = nullptr;

        switch (currentState) {
        case CANDLE_OFF:
            currentTextures = &candleOffTextures;
            break;
        case SHOW_CANDLE:
            currentTextures = &showCandleTextures;
            break;
        case CANDLE_ON:
            currentTextures = &candleOnTextures;
            break;
        case BLOW_CANDLE:
            currentTextures = &blowCandleTextures;
            break;
        }

        if (currentTextures && animationFrame >= currentTextures->size()) {
            if (currentState == SHOW_CANDLE) {
                switchState(CANDLE_ON); // Sau SHOW_CANDLE, chuyển sang CANDLE_ON
            }
            else if (currentState == BLOW_CANDLE) {
                switchState(CANDLE_OFF); // Sau BLOW_CANDLE, chuyển sang CANDLE_OFF
            }
            else {
                animationFrame = 0; // Lặp lại frame
            }
        }
    }
}

void Penguin::render(SDL_Renderer* renderer) {
    const std::vector<SDL_Texture*>* currentTextures = nullptr;

    switch (currentState) {
    case CANDLE_OFF: currentTextures = &candleOffTextures; break;
    case SHOW_CANDLE: currentTextures = &showCandleTextures; break;
    case CANDLE_ON: currentTextures = &candleOnTextures; break;
    case BLOW_CANDLE: currentTextures = &blowCandleTextures; break;
    }

    if (currentTextures && !currentTextures->empty()) {
        SDL_RenderCopy(renderer, (*currentTextures)[animationFrame % currentTextures->size()], nullptr, &position);
    }
}

void Penguin::setFrameDuration(int duration) {
    frameDuration = duration;
}

bool Penguin::isMouseHovering(int mouseX, int mouseY) {
    return mouseX >= position.x && mouseX <= position.x + position.w &&
        mouseY >= position.y && mouseY <= position.y + position.h;
}

Penguin::State Penguin::getCurrentState() {
    return currentState;
}

void Penguin::setState(State state) {
    currentState = state;
}

