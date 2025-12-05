#include "audioPlayer.h"
#include <FS.h>
#include <SPIFFS.h>
#include "driver/dac.h"

hw_timer_t *audioTimer = nullptr;
volatile bool playing = false;
volatile size_t audioIndex = 0;
File audioFile;
uint8_t audioSample = 128;  // silence

// ISR runs at sample rate
void IRAM_ATTR onTimer() {
    if (!playing || !audioFile) {
        dac_output_voltage(AUDIO_DAC_PIN, 128); // silence
        return;
    }

    // Only play when pin is HIGH
    if (digitalRead(AUDIO_PLAY_PIN) == HIGH) {
        if (audioFile.available()) {
            audioSample = audioFile.read();
            dac_output_voltage(AUDIO_DAC_PIN, audioSample);
        } else {
            playing = false;
            dac_output_voltage(AUDIO_DAC_PIN, 128); // silence
            audioFile.close();
        }
    } else {
        dac_output_voltage(AUDIO_DAC_PIN, 128); // silence
    }
}

void audioInit() {
    pinMode(AUDIO_PLAY_PIN, INPUT);
    dac_output_enable(AUDIO_DAC_PIN);

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    // 8 kHz timer
    audioTimer = timerBegin(8000);
    timerAttachInterrupt(audioTimer, &onTimer);
    timerStart(audioTimer);
}

void audioPlay(const char* filename) {
    if (playing) audioFile.close();

    audioFile = SPIFFS.open(filename, "r");
    if (!audioFile) {
        Serial.println("Failed to open audio file");
        playing = false;
        return;
    }

    audioIndex = 0;
    playing = true;
}

void audioLoop() {
    // Non-blocking, all handled in ISR
}

bool audioIsPlaying() {
    return playing;
}
