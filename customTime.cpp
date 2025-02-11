#include "customTime.h"

customTime::customTime(SDL_Renderer* renderer, TTF_Font* font, Penguin* penguin, std::string& targetTime)
    : renderer(renderer), font(font), penguin(penguin), isRender(true), texture(nullptr), targetTime(targetTime) {
    if (penguin == nullptr) {
        std::cerr << "Error: Penguin pointer is null!" << std::endl;
        return;  // Nếu penguin là nullptr, không thể tiếp tục
    }

    GetLocalTime(&st);
    time = SystemTimeToString(st);
    setTexture();
}

customTime::~customTime() {
    // Tránh việc hủy texture nếu nó không tồn tại
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
    // Kiểm tra nếu penguin không phải là null
    if (penguin == nullptr) {
        return;
    }

    // Đổi trạng thái render nếu cần
    if (penguin->getCurrentState() == Penguin::SHOW_CANDLE) {
        isRender = false;
    }

    if (!isRender) return;

    SYSTEMTIME newTime;
    GetLocalTime(&newTime);
    std::string newTimeStr = SystemTimeToString(newTime);

    // Nếu thời gian thay đổi, cập nhật lại texture
    if (newTimeStr != time) {
        time = newTimeStr;

        if (time == targetTime) {
            penguin->setState(Penguin::SHOW_CANDLE);
            isRender = false;
        }

        setTexture();
    }
}

void customTime::render() {
    // Nếu không cần render hoặc texture không hợp lệ, thoát
    if (!isRender || !texture) {
        return;
    }

    SDL_Rect destRect = { (800 - textWidth) / 2, (600 - textHeight) / 2, textWidth, textHeight };
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

void customTime::setTexture() {
    // Tránh việc hủy texture nếu nó không tồn tại
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    SDL_Color color = { 246, 190, 239, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, time.c_str(), color);

    // Kiểm tra lỗi khi tạo surface
    if (!surface) {
        std::cerr << "Error creating surface: " << TTF_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Kiểm tra lỗi khi tạo texture
    if (!texture) {
        std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);
}
