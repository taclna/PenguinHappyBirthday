#pragma once

#include "IVolumeProcessor.h"
#include "Penguin.h"

class PenguinVolumeProcessor : public IVolumeProcessor {
private:
    Penguin* penguin;

public:
    explicit PenguinVolumeProcessor(Penguin* penguinInstance) : penguin(penguinInstance) {}
    void processVolume(float volume) override {
        if (penguin) {
            penguin->setVolume(volume); // Gửi giá trị volume cho lớp Penguin
        }
    }
};

