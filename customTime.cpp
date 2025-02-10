#include "customTime.h"

customTime::customTime(SDL_Renderer* renderer, TTF_Font* font, Penguin* penguin)
    : renderer(renderer), font(font), penguin(penguin), isRender(true), texture(nullptr) {
    GetLocalTime(&st);
    time = SystemTimeToString(st);
    setTexture();
}

customTime::~customTime() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

std::string customTime::SystemTimeToString(const SYSTEMTIME& st) {
    std::ostringstream oss;
    oss << (st.wHour < 10 ? "0" : "") << st.wHour
        << ":" << (st.wMinute < 10 ? "0" : "") << st.wMinute
        << ":" << (st.wSecond < 10 ? "0" : "") << st.wSecond;
    return oss.str();
}

void customTime::update() {
    if (penguin->getCurrentState() == Penguin::SHOW_CANDLE) {
        isRender = false;
    }

    if (!isRender) return;

    SYSTEMTIME newTime;
    GetLocalTime(&newTime);
    std::string newTimeStr = SystemTimeToString(newTime);

    if (newTimeStr != time) {
        time = newTimeStr;

        if (time == "23:52:00") {
            penguin->setState(Penguin::SHOW_CANDLE);
            isRender = false;
        }

        setTexture();
    }
}

void customTime::render() {
    if (!isRender || !texture) {
        return;
    }
    SDL_Rect destRect = {(800 - textWidth) / 2, (600 - textHeight) / 2, textWidth, textHeight};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

void customTime::setTexture() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    SDL_Color color = { 246, 190, 239, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, time.c_str(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);
}
