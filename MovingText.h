#ifndef MOVINGTEXT_H
#define MOVINGTEXT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class MovingText {
public:
    MovingText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int targetX, int targetY);
    ~MovingText();

    void update();
    void render();

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Texture* texture;
    std::string text;

    int targetX, targetY;
    int yPos;
    int textWidth, textHeight;
    float amplitude;
    float frequency;
    float elapsed;

    Uint32 lastUpdateTime;

    float getDeltaTime();
};

#endif // MOVINGTEXT_H
