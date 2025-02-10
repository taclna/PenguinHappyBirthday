#include "MicrophoneCapture.h"
#include <iostream>
#include <cmath>
#include <chrono>

MicrophoneCapture::MicrophoneCapture() = default;

MicrophoneCapture::~MicrophoneCapture() {
    stop();
    cleanup();
}

bool MicrophoneCapture::initialize() {
    CoInitialize(nullptr);

    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        std::cerr << "CoCreateInstance failed" << std::endl;
        return false;
    }

    hr = pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);
    if (FAILED(hr)) {
        std::cerr << "GetDefaultAudioEndpoint failed" << std::endl;
        return false;
    }

    hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&pAudioClient);
    if (FAILED(hr)) {
        std::cerr << "Activate IAudioClient failed" << std::endl;
        return false;
    }

    hr = pAudioClient->GetMixFormat(&pFormat);
    if (FAILED(hr)) {
        std::cerr << "GetMixFormat failed" << std::endl;
        return false;
    }

    hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 10000000, 0, pFormat, nullptr);
    if (FAILED(hr)) {
        std::cerr << "Initialize IAudioClient failed" << std::endl;
        return false;
    }

    hr = pAudioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&pCaptureClient);
    if (FAILED(hr)) {
        std::cerr << "GetService IAudioCaptureClient failed" << std::endl;
        return false;
    }

    return true;
}

void MicrophoneCapture::setVolumeProcessor(IVolumeProcessor* processor) {
    volumeProcessor = processor;
}

void MicrophoneCapture::start() {
    if (isRunning) return;

    HRESULT hr = pAudioClient->Start();
    if (FAILED(hr)) {
        std::cerr << "AudioClient Start failed" << std::endl;
        return;
    }

    isRunning = true;
    captureThread = std::thread(&MicrophoneCapture::captureLoop, this);
}

void MicrophoneCapture::stop() {
    if (!isRunning) return;

    isRunning = false;
    if (captureThread.joinable()) {
        captureThread.join();
    }

    pAudioClient->Stop();
}

void MicrophoneCapture::cleanup() {
    if (pCaptureClient) pCaptureClient->Release();
    if (pAudioClient) pAudioClient->Release();
    if (pDevice) pDevice->Release();
    if (pEnumerator) pEnumerator->Release();
    if (pFormat) CoTaskMemFree(pFormat);
    CoUninitialize();
}

void MicrophoneCapture::captureLoop() {
    while (isRunning) {
        UINT32 packetSize = 0;
        HRESULT hr = pCaptureClient->GetNextPacketSize(&packetSize);
        if (FAILED(hr)) {
            std::cerr << "GetNextPacketSize failed" << std::endl;
            break;
        }

        if (packetSize == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        BYTE* pData;
        UINT32 numFramesAvailable;
        DWORD flags;
        hr = pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, nullptr, nullptr);
        if (FAILED(hr)) {
            std::cerr << "GetBuffer failed" << std::endl;
            continue;
        }

        float volume = 0.0f;
        for (UINT32 i = 0; i < numFramesAvailable; i++) {
            short* pSample = reinterpret_cast<short*>(pData + i * 2);
            volume += std::abs(*pSample);
        }

        volume /= numFramesAvailable;
        volume = min(volume, 32767.0f);
        float normalizedVolume = (volume / 32767.0f) * 100.0f;

        if (volumeProcessor) {
            volumeProcessor->processVolume(normalizedVolume);
        }

        hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
        if (FAILED(hr)) {
            std::cerr << "ReleaseBuffer failed" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
