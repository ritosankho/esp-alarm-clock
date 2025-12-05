#pragma once
#include <Arduino.h>

#define AUDIO_PLAY_PIN 32  // Only play audio when this GPIO is HIGH
#define AUDIO_DAC_PIN DAC_CHANNEL_1  // DAC_CHAN_1 = GPIO 26

// Initialize audio system
void audioInit();

// Start playback of a WAV file stored in SPIFFS
void audioPlay(const char* filename);

// Call in loop if needed (empty for now)
void audioLoop();

// Check if audio is playing
bool audioIsPlaying();
