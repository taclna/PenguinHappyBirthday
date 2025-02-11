#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include "Penguin.h"
#include "PeguinVolumeProcessor.h"
#include "MicrophoneCapture.h"
#include "Background.h"
#include "MovingText.h"
#include "CircularImageViewer.h"
#include "MessageDisplay.h"
#include "Present.h"
#include "customTime.h"
#include "Gift.h"


#define handCursor SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)
#define defaultCursor SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW)

#define screenWidth 800
#define screenHeight 600

std::string getNameFromFile(std::string filePath) {
    std::ifstream file(filePath);
    std::string line;

    if (!file) {
        std::cerr << "Không thể mở file! " << filePath << std::endl;
    }
    if (file.is_open()) {
        
        std::getline(file, line);
        file.close();
    }
    else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
    return line;
}

// Function to load images from a directory
std::vector<std::string> loadImagesFromDirectory(const std::string& directory) {
    std::vector<std::string> images;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            images.push_back(entry.path().string());
        }
    }
    return images;
}

std::string loadImgFromDirectory(const std::string& directory) {
    std::vector<std::string> images;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            images.push_back(entry.path().string());
        }
    }
    return images[0];
}

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
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

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Initialize SDL_Image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_Mixer: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return -1;
    }

    // Create SDL Window
    SDL_Window* window = SDL_CreateWindow("Penguin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Create SDL Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // Tải nhạc nền
    Mix_Music* backgroundMusic = Mix_LoadMUS("data\\happytoyou.mp3");
    if (!backgroundMusic) {
        std::cerr << "Failed to load background music: " << Mix_GetError() << std::endl;
        Mix_Quit();
        SDL_Quit();
        return -1;
    }

    // Phát nhạc nền (lặp vô hạn)
    if (Mix_PlayMusic(backgroundMusic, -1) == -1) {
        std::cerr << "Failed to play background music: " << Mix_GetError() << std::endl;
        Mix_FreeMusic(backgroundMusic);
        Mix_Quit();
        SDL_Quit();
        return -1;
    }

    // Load textures for the penguin from directories
    std::vector<std::string> candleOffImgs = loadImagesFromDirectory("data\\CANDLE_OFF");
    std::vector<std::string> candleOnImgs = loadImagesFromDirectory("data\\CANDLE_ON");
    std::vector<std::string> blowCandleImgs = loadImagesFromDirectory("data\\BLOW_CANDLE");
    std::vector<std::string> showCandleImgs = loadImagesFromDirectory("data\\SHOW_CANDLE");

    std::vector<std::string> presentImgs = loadImagesFromDirectory("data\\Present");

    std::string giftPath = loadImgFromDirectory("data\\Custom\\Gift");

    TTF_Font* font = TTF_OpenFont("data\\JingleBalonsGTDemo.ttf", 80);
    TTF_Font* messageFont = TTF_OpenFont("data\\Dongle-Bold.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return -1;
    }

    std::string name = getNameFromFile("data\\Custom\\name.txt");
    std::string targetTime = getNameFromFile("data\\Custom\\time.txt");

    MessageDisplay message(renderer, "data\\Custom\\message.txt", messageFont, 500, 500);
    Penguin penguin(renderer,&message,  candleOffImgs, candleOnImgs, blowCandleImgs, showCandleImgs);

    PenguinVolumeProcessor volumeProcessor(&penguin);
    Background background(renderer, "data\\background1.png", { 0, 0, screenWidth, screenHeight });
    CircularImageViewer viewer(renderer, 170, 460, 100, 20, 0.5, 3000);

    Present present(renderer, presentImgs, "data\\39026__wildweasel__keypickup.wav", {490, 350, 250, 250}, &message, &penguin);

    MovingText movingText(renderer, font, name, 400, 325);
    customTime customTime(renderer, font,&penguin, targetTime);
    Gift gift(renderer, giftPath.c_str(), &present, 520, 450, 50);


    MicrophoneCapture microphoneCapture;
    microphoneCapture.setVolumeProcessor(&volumeProcessor);

    std::vector<std::string> circularImages = loadImagesFromDirectory("data\\Custom\\Person");
    viewer.addImages(circularImages);

    if (!microphoneCapture.initialize()) {
        return -1;
    }

    microphoneCapture.start();

    bool isRunning = true;
    SDL_Event e;
    penguin.setFrameDuration(70);


    while (isRunning) {
        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT) {
                isRunning = false;
            }

            if (e.type == SDL_MOUSEMOTION) {
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;

                if (penguin.isMouseHovering(mouseX, mouseY) || present.isMouseInside(mouseX, mouseY) || gift.isMouseInside()) {
                    SDL_SetCursor(handCursor); 
                }
                else {
                    SDL_SetCursor(defaultCursor); 
                }

                
            }

            if (present.isRendering()){
                present.handleEvent(e);
            }

            penguin.handleEvent(e);
            gift.handleEvent(e);
        }

        penguin.update();
        background.update(penguin.getCurrentState());
        movingText.update();
        viewer.update();
        customTime.update();
        gift.update();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        
        background.render(renderer);
        penguin.render(renderer);
        customTime.render();
        message.update();
        message.render();
        if (background.checkImgShown()) {
            movingText.render();
            viewer.render();
            if (message.checkRendering() == false) {
                message.setRender();
            }
            
        }
        present.update();
        present.render();
        gift.render(renderer);

        SDL_RenderPresent(renderer);
    }

    Mix_FreeMusic(backgroundMusic);

    font = nullptr;
    messageFont = nullptr;
    try {
        penguin.~Penguin();
        background.~Background();
        movingText.~MovingText();
        viewer.~CircularImageViewer();
        microphoneCapture.stop();
        microphoneCapture.~MicrophoneCapture();
        present.~Present();
        customTime.~customTime();
    }
    catch (std::exception e) {

    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    Mix_Quit();

    return 0;
}
