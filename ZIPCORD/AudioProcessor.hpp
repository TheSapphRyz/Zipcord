#pragma once
#include "portaudio.h"
#include "main.h"
#include <iostream>

class AudioProcessor {
public:
    AudioProcessor() : stream(nullptr), frameIndex(0) {}

    bool start();
    void stop();

private:
    PaStream* stream;
    unsigned long frameIndex;
    int memberCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags);
    static int paCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
};


