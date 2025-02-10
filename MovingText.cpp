#include "MovingText.h"
#include <cmath>

MovingText::MovingText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int targetX, int targetY)
    : renderer(renderer), font(font), text(text), targetX(targetX), targetY(targetY) {
    amplitude = 10; // Biên độ dao động
    frequency = 5;  // Tần số dao động
    elapsed = 0;    // Thời gian đã trôi qua

    SDL_Color color = { 246, 190, 239, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);

    lastUpdateTime = SDL_GetTicks(); // Khởi tạo theo thời gian hiện tại
}

MovingText::~MovingText() {
    SDL_DestroyTexture(texture);
}

void MovingText::update() {
    float deltaTime = getDeltaTime();
    elapsed += deltaTime; // Cập nhật thời gian đã trôi qua để tính toán dao động
}

void MovingText::render() {
    // Tính toán vị trí dao động theo biên độ và tần số
    int displayY = targetY + static_cast<int>(amplitude * sin(elapsed * frequency));

    SDL_Rect destRect = { targetX - textWidth / 2, displayY - textHeight / 2, textWidth, textHeight };
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

float MovingText::getDeltaTime() {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastUpdateTime) / 1000.0f; // Chuyển đổi thời gian thành giây
    lastUpdateTime = currentTime;
    return deltaTime;
}
