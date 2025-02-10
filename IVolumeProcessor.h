#ifndef I_VOLUME_PROCESSOR_H
#define I_VOLUME_PROCESSOR_H

class IVolumeProcessor {
public:
    virtual void processVolume(float volume) = 0; // Phương thức thuần ảo
    virtual ~IVolumeProcessor() = default;       // Destructor ảo
};

#endif // I_VOLUME_PROCESSOR_H
