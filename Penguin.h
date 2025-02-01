#ifndef PENGUIN_H
#define PENGUIN_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>

#define screenWidth 800
#define screenHeight 600

class Penguin {
public:
    enum State {
        CANDLE_OFF,
        SHOW_CANDLE,
        CANDLE_ON,
        BLOW_CANDLE
    };

    Penguin(SDL_Renderer* renderer, const std::vector<std::string>& candleOffImgs,
        const std::vector<std::string>& candleOnImgs, const std::vector<std::string>& blowCandleImgs,
        const std::vector<std::string>& showCandleImgs);
    ~Penguin();

    void handleEvent(SDL_Event& e);
    void update();
    void render(SDL_Renderer* renderer);
    void setVolume(float volume); // Nhận giá trị âm lượng từ MicrophoneCapture
    void setFrameDuration(int duration);
    bool isMouseHovering(int mouseX, int mouseY);
    State getCurrentState();


private:
    void switchState(State newState);

    State currentState;
    std::vector<SDL_Texture*> candleOffTextures;
    std::vector<SDL_Texture*> candleOnTextures;
    std::vector<SDL_Texture*> blowCandleTextures;
    std::vector<SDL_Texture*> showCandleTextures;

    SDL_Rect position;
    int animationFrame;
    Uint32 lastFrameTime;
    int frameDuration;
    float currentVolume; // Biến lưu giá trị âm lượng

    SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);

};

#endif
