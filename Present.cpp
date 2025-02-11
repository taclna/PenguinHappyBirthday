#include "Present.h"

// Constructor
Present::Present(SDL_Renderer* renderer, const std::vector<std::string>& imagePaths, const std::string& soundPath, SDL_Rect position, MessageDisplay* message, Penguin* penguin)
    : renderer(renderer), currentImageIndex(0), isAnimating(false), lastUpdateTime(0), soundEffect(nullptr), position(position), message(message), penguin(penguin), isRender(false)
      , isDone(false) {

    // Load images
    for (const std::string& path : imagePaths) {
        SDL_Texture* texture = loadTexture(path);
        if (texture) {
            textures.push_back(texture);
        }
    }

    // Load sound
    if (!soundPath.empty()) {
        soundEffect = Mix_LoadWAV(soundPath.c_str());
        if (!soundEffect) {
            std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
        }
    }
}

// Destructor
Present::~Present() {
    for (SDL_Texture* texture : textures) {
        SDL_DestroyTexture(texture);
    }
    if (soundEffect) {
        Mix_FreeChunk(soundEffect);
    }
}

// Load texture from file
SDL_Texture* Present::loadTexture(const std::string& path) {
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

// Check if mouse click is inside present's area
bool Present::isMouseInside(int x, int y) {
    return (x > position.x && x < position.x + position.w && y > position.y && y < position.y + position.h);
}

// Handle user input (click event)
void Present::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN && !isAnimating) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if (isMouseInside(mouseX, mouseY)) {
            isAnimating = true;
            playSound();
            lastUpdateTime = SDL_GetTicks();
        }
    }
}

// Update image animation
void Present::update() {
    if (!message->isFinished() || !(penguin->getCurrentState() == Penguin::CANDLE_OFF)) return;
    if (isAnimating) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastUpdateTime > 100) { // 500ms delay between frames
            if (currentImageIndex < textures.size() - 1) {
                currentImageIndex++;
                lastUpdateTime = currentTime;
            }
            else {
                isDone = true;
                isAnimating = false;
            }
        }
    }
}

// Render the present
void Present::render() {
    if (!message->isFinished() || !(penguin->getCurrentState() == Penguin::CANDLE_OFF)) return;
    if (!isRender) {
        playSound();
        isRender = true;
    }
    if (!textures.empty()) {
        SDL_RenderCopy(renderer, textures[currentImageIndex], NULL, &position);
    }
}

// Play sound effect
void Present::playSound() {
    if (soundEffect) {
        Mix_PlayChannel(-1, soundEffect, 0);
    }
}

// Check if present is running animation
bool Present::isRunning() const {
    return isAnimating;
}

bool Present::isRendering() {
    return isRender;
}

bool Present::checkDone() {
    return isDone;
}
