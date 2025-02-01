#ifndef CIRCULAR_IMAGE_VIEWER_H
#define CIRCULAR_IMAGE_VIEWER_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

class CircularImageViewer {
private:
    SDL_Renderer* renderer;
    std::vector<SDL_Texture*> textures;
    int centerX, centerY; // Center position of the circle
    int radius;          // Radius of the circle
    int currentImageIndex;
    int imageDisplayDuration; // Duration to display each image (ms)
    int oscillationAmplitude; // Amplitude of oscillation
    int oscillationFrequency; // Frequency of oscillation (cycles/ms)
    Uint32 lastImageSwitchTime;
    Uint32 startTime;

public:
    // Constructor
    CircularImageViewer(SDL_Renderer* renderer, int x, int y, int radius, int amplitude, int frequency, int displayDuration);

    // Destructor
    ~CircularImageViewer();

    // Add image paths to the viewer
    void addImages(const std::vector<std::string>& imagePaths);

    // Update the viewer state
    void update();

    // Render the current image in the circle
    void render();
};

#endif
