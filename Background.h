#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Penguin.h"

class Background {
private:
    SDL_Texture* imageTexture;    // Texture của ảnh nền
    SDL_Rect screenSize;          // Kích thước màn hình
    int transitionRadius;         // Bán kính hiệu ứng chuyển đổi
    bool transitioning;           // Có đang trong quá trình chuyển đổi không
    bool toBlack;                 // Xác định hướng chuyển đổi (sang đen hay sang ảnh)
    Penguin::State currentState;  // Trạng thái hiện tại của Penguin
    bool imageShown;

    SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);

public:
    // Constructor và Destructor
    Background(SDL_Renderer* renderer, const std::string& imagePath, SDL_Rect screen);
    ~Background();

    // Cập nhật trạng thái dựa trên trạng thái của Penguin
    void update(Penguin::State newState);

    // Render nền ra màn hình
    void render(SDL_Renderer* renderer);

    bool checkImgShown();
};

#endif
