#include "MessageDisplay.h"
#include <Windows.h>

// Constructor: Loads messages from the file
MessageDisplay::MessageDisplay(SDL_Renderer* renderer, const std::string& filePath, TTF_Font* font, int x, int y)
    : renderer(renderer), font(font), posX(x), posY(y), currentMessageIndex(0), finished(false), lastTime(0), currentTime(0), timeDelta(3000), isRender(false) {
    loadMessages(filePath);
}

// Destructor: Free resources
MessageDisplay::~MessageDisplay() {
    // No resources to free in this class since the messages are simple strings
}

// Load messages from file
void MessageDisplay::loadMessages(const std::string& filePath) {
    // Đọc file với mã hóa UTF-8
    std::ifstream file(filePath);

    if (!file) {
        std::cerr << "Không thể mở file!" << std::endl;
    }
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            messages.push_back(line);
        }
        file.close();
    }
    else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
}

// Handle events (in this case, just simple mouse clicks or keyboard events can be checked)
void MessageDisplay::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_KEYDOWN) {
        if (currentMessageIndex < messages.size()) {
            currentMessageIndex++;
        }
    }
}

// Chuyển std::wstring thành std::string (UTF-8)
std::string MessageDisplay:: wstring_to_string(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, nullptr, nullptr);
    return str;
}

// Render the current message
void MessageDisplay::render() {
    if (!isRender) return;
    if (currentMessageIndex < messages.size()) {
        SDL_Color textColor = { 246, 190, 239, 255 };  // White text
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, messages[currentMessageIndex].c_str(), textColor);
        if (textSurface != nullptr) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);

            if (textTexture != nullptr) {
                int textWidth = 0, textHeight = 0;
                SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
                SDL_Rect renderQuad = { posX, posY, textWidth, textHeight };
                SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
                SDL_DestroyTexture(textTexture);
            }
        }
    }
}

// Check if all messages are displayed
bool MessageDisplay::isFinished() {
    return currentMessageIndex >= messages.size();
}

void MessageDisplay::update() {
    if (!isRender) {
        lastTime = SDL_GetTicks();
        return;
    }
    currentTime = SDL_GetTicks();
    if (currentTime - lastTime >= timeDelta) {
        if (currentMessageIndex < messages.size()) {
            currentMessageIndex++;
        }
        lastTime = currentTime;
    }
}

bool MessageDisplay::checkRendering() {
    return isRender;
}

void MessageDisplay::setRender() {
    isRender = true;
}
