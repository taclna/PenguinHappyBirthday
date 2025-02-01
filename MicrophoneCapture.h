#ifndef MICROPHONE_CAPTURE_H
#define MICROPHONE_CAPTURE_H

#include "IVolumeProcessor.h"
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <thread>
#include <atomic>

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Oleaut32.lib")

class MicrophoneCapture {
private:
    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioClient* pAudioClient = nullptr;
    IAudioCaptureClient* pCaptureClient = nullptr;
    WAVEFORMATEX* pFormat = nullptr;
    std::atomic<bool> isRunning{ false };
    std::thread captureThread;

    IVolumeProcessor* volumeProcessor = nullptr;

    void captureLoop();

public:
    MicrophoneCapture();
    ~MicrophoneCapture();

    bool initialize();
    void setVolumeProcessor(IVolumeProcessor* processor);
    void start();
    void stop();
    void cleanup();
};

#endif // MICROPHONE_CAPTURE_H
