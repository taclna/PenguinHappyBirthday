#ifndef VOLUME_PROCESSOR_H
#define VOLUME_PROCESSOR_H

#include "IVolumeProcessor.h"
#include <iostream>

class VolumeProcessor : public IVolumeProcessor {
public:
    void processVolume(float volume) override;
};

#endif // VOLUME_PROCESSOR_H
