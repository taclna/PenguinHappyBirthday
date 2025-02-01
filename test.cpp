#include <iostream>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <endpointvolume.h>
#include <windows.h>
#include <functiondiscoverykeys_devpkey.h>

DEFINE_GUID(IID_IAudioMeterInformation,
    0xc02216f6, 0x8c67, 0x4b5b, 0x9d, 0x00, 0xd0, 0x16, 0xb2, 0x3e, 0x8e, 0x5f);

#pragma comment(lib, "ole32.lib")



// Hàm kiểm tra âm lượng microphone
bool CheckMicrophoneVolume(float threshold)
{
    HRESULT hr;
    bool isAboveThreshold = false;

    CoInitialize(nullptr);

    IMMDeviceEnumerator* pDeviceEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioMeterInformation* pMeterInfo = nullptr;

    float peakVolume = 0.0f; // Chuyển khai báo biến lên đầu

    // Tạo Device Enumerator
    hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator),
        nullptr,
        CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator),
        (void**)&pDeviceEnumerator);
    if (FAILED(hr)) {
        std::cerr << "Failed to create device enumerator." << std::endl;
        goto cleanup;
    }

    // Lấy default audio capture device
    hr = pDeviceEnumerator->GetDefaultAudioEndpoint(
        eCapture,
        eCommunications,
        &pDevice);
    if (FAILED(hr)) {
        std::cerr << "Failed to get default audio endpoint." << std::endl;
        goto cleanup;
    }

    // Lấy Audio Meter Information
    hr = pDevice->Activate(
        __uuidof(IAudioMeterInformation),
        CLSCTX_ALL,
        nullptr,
        (void**)&pMeterInfo);
    if (FAILED(hr)) {
        std::cerr << "Failed to activate audio meter information." << std::endl;
        goto cleanup;
    }

    // Lấy âm lượng hiện tại
    hr = pMeterInfo->GetPeakValue(&peakVolume);
    if (FAILED(hr)) {
        std::cerr << "Failed to get peak value." << std::endl;
        goto cleanup;
    }

    std::cout << "Current microphone volume: " << peakVolume << std::endl;
    if (peakVolume > threshold) {
        isAboveThreshold = true;
    }

cleanup:
    if (pMeterInfo) pMeterInfo->Release();
    if (pDevice) pDevice->Release();
    if (pDeviceEnumerator) pDeviceEnumerator->Release();
    CoUninitialize();

    return isAboveThreshold;
}

int main()
{
    const float volumeThreshold = 0.5f; // Ngưỡng âm lượng
    std::cout << "Monitoring microphone volume. Press Ctrl+C to exit." << std::endl;

    while (true) {
        if (CheckMicrophoneVolume(volumeThreshold)) {
            std::cout << "Microphone volume exceeded threshold! Stopping program." << std::endl;
            break;
        }
        Sleep(500); // Dừng 500ms trước khi kiểm tra lại
    }

    return 0;
}
