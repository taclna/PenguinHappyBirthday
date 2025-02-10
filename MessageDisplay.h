#ifndef MESSAGEDISPLAY_H
#define MESSAGEDISPLAY_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <Windows.h>

#define screenWidth 800
#define screenHeight 600

class MessageDisplay {
public:
    MessageDisplay(SDL_Renderer* renderer, const std::string& filePath, TTF_Font* font, int x, int y);
    ~MessageDisplay();

    void handleEvent(SDL_Event& e);
    void render();
    bool isFinished();
    void update();
    std::string wstring_to_string(const std::wstring& wstr);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<std::string> messages;
    int currentMessageIndex;
    int posX, posY;
    Uint32 lastTime, currentTime, timeDelta;
    bool finished;

    // Helper function to load messages from file
    void loadMessages(const std::string& filePath);
};

#endif // MESSAGEDISPLAY_H
