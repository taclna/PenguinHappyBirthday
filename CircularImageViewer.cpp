#include "CircularImageViewer.h"

CircularImageViewer::CircularImageViewer(SDL_Renderer* renderer, int x, int y, int radius, int amplitude, int frequency, int displayDuration)
    : renderer(renderer), centerX(x), centerY(y), radius(radius),
    oscillationAmplitude(amplitude), oscillationFrequency(frequency),
    imageDisplayDuration(displayDuration), currentImageIndex(0) {
    lastImageSwitchTime = SDL_GetTicks();
    startTime = SDL_GetTicks();
}

CircularImageViewer::~CircularImageViewer() {
    for (auto texture : textures) {
        SDL_DestroyTexture(texture);
    }
}

void CircularImageViewer::addImages(const std::vector<std::string>& imagePaths) {
    for (const auto& path : imagePaths) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            std::cerr << "Failed to load image: " << path << " " << IMG_GetError() << std::endl;
            continue;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (texture) {
            textures.push_back(texture);
        }
    }
}

void CircularImageViewer::update() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastImageSwitchTime >= imageDisplayDuration) {
        currentImageIndex = (currentImageIndex + 1) % textures.size();
        lastImageSwitchTime = currentTime;
    }
}

void CircularImageViewer::render() {
    if (textures.empty()) return;

    // Tính toán vị trí dao động
    // Tính toán dao động hỗn độn
    Uint32 currentTime = SDL_GetTicks() - startTime;

    // Dao động theo trục X và Y với các hàm sin/cos độc lập
    int oscillationOffsetX = static_cast<int>(oscillationAmplitude *
        std::sin(2 * M_PI * oscillationFrequency * currentTime / 1000.0));
    int oscillationOffsetY = static_cast<int>(oscillationAmplitude *
        std::cos(2 * M_PI * (oscillationFrequency + 0.5) * currentTime / 1000.0));

    // Thêm nhiễu ngẫu nhiên nhỏ vào dao động
    int randomNoiseX = 0; // Nhiễu trong khoảng [-2, 2]
    int randomNoiseY = 0; // Nhiễu trong khoảng [-2, 2]

    // Tọa độ dao động cuối cùng
    int posX = centerX + oscillationOffsetX + randomNoiseX;
    int posY = centerY + oscillationOffsetY + randomNoiseY;


    // Tạo texture mặt nạ
    SDL_Texture* maskTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, radius * 2, radius * 2);
    SDL_SetTextureBlendMode(maskTexture, SDL_BLENDMODE_BLEND);

    // Thiết lập render target thành texture mặt nạ
    SDL_SetRenderTarget(renderer, maskTexture);

    // Xóa texture mặt nạ với màu trong suốt
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Trong suốt
    SDL_RenderClear(renderer);

    // Vẽ hình tròn màu trắng để tạo vùng hiển thị
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Màu trắng đầy đủ
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, w, h);
            }
        }
    }

    // Đặt render target về mặc định
    SDL_SetRenderTarget(renderer, nullptr);

    // Áp dụng mặt nạ lên ảnh
    SDL_Texture* maskedImage = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, radius * 2, radius * 2);
    SDL_SetTextureBlendMode(maskedImage, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, maskedImage);

    // Vẽ ảnh gốc
    SDL_Rect destRect = { 0, 0, radius * 2, radius * 2 };
    SDL_RenderCopy(renderer, textures[currentImageIndex], nullptr, &destRect);

    // Áp dụng mặt nạ
    SDL_SetTextureBlendMode(maskTexture, SDL_ComposeCustomBlendMode(
        SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, // RGB giữ nguyên
        SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDOPERATION_ADD // Alpha dựa trên mặt nạ
    ));
    SDL_RenderCopy(renderer, maskTexture, nullptr, &destRect);

    // Đặt render target về màn hình chính
    SDL_SetRenderTarget(renderer, nullptr);

    // Vẽ kết quả cuối cùng
    SDL_Rect finalDestRect = { posX - radius, posY - radius, radius * 2, radius * 2 };
    SDL_RenderCopy(renderer, maskedImage, nullptr, &finalDestRect);

    // Giải phóng texture tạm
    SDL_DestroyTexture(maskTexture);
    SDL_DestroyTexture(maskedImage);
}



