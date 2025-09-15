#include "AudioProcessor.hpp"
#include "portaudio.h"
#include "main.h"
#include <iostream>
#include <cstring>
#include <string>
PaStream* stream;
unsigned long frameIndex;

int AudioProcessor::memberCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags) {
    const float* in = static_cast<const float*>(inputBuffer);
    float* out = static_cast<float*>(outputBuffer);

    if (inputBuffer == nullptr) {
        memset(out, 0, framesPerBuffer * NUM_CHANNELS * sizeof(float));
    }
    else {
        // ѕроста€ передача аудио без обработки
        memcpy(out, in, framesPerBuffer * NUM_CHANNELS * sizeof(float));
    }

    frameIndex += framesPerBuffer;
    return paContinue;
}

int AudioProcessor::paCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags, void* userData) {
    return static_cast<AudioProcessor*>(userData)->memberCallback(
        inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags);
}

bool AudioProcessor::start() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio init error: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }

    err = Pa_OpenDefaultStream(&stream, NUM_CHANNELS, NUM_CHANNELS, paFloat32,
        SAMPLE_RATE, FRAMES_PER_BUFFER, paCallback, this);
    if (err != paNoError) {
        std::cerr << "PortAudio open stream error: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return false;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio start stream error: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return false;
    }

    std::cout << "Audio stream started." << std::endl;
    return true;
}

void AudioProcessor::stop() {
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
        stream = nullptr;
        Pa_Terminate();
    }
}