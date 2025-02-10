#include "Ice.h"

Ice::Ice(SDL_Renderer* renderer, const std::vector<std::string>& iceImgs)
        : renderer(renderer) {
	for (const auto& img : iceImgs) {
        iceTextures.push_back(loadTexture(img));
	}
}

Ice::~Ice() {
    for (auto& texture : iceTextures) SDL_DestroyTexture(texture);
}

SDL_Texture* Ice::loadTexture(const std::string& path) {
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
