#ifndef SERVOESP32_H
#define SERVOESP32_H

#include <Arduino.h>
#include "driver/ledc.h"

class ServoESP32 {
public:
    ServoESP32()
        : pin(-1), channel(LEDC_CHANNEL_0),
          minUs(500), maxUs(2400), freq(50) {}

    // Attach servo to a pin and LEDC channel
    bool attach(int pinNumber,
                int channelNumber = 0,
                int minPulse = 500,
                int maxPulse = 2400)
    {
        pin = pinNumber;
        channel = (ledc_channel_t)channelNumber;
        minUs = minPulse;
        maxUs = maxPulse;

        // TIMER CONFIG (Core 3.x LEDC API)
        ledc_timer_config_t timerConf = {
            .speed_mode       = LEDC_LOW_SPEED_MODE,
            .duty_resolution  = LEDC_TIMER_16_BIT,
            .timer_num        = LEDC_TIMER_0,
            .freq_hz          = (uint32_t)freq,
            .clk_cfg          = LEDC_AUTO_CLK
        };
        if (ledc_timer_config(&timerConf) != ESP_OK)
            return false;

        // CHANNEL CONFIG
        ledc_channel_config_t channelConf = {
            .gpio_num   = pin,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel    = channel,
            .intr_type  = LEDC_INTR_DISABLE,
            .timer_sel  = LEDC_TIMER_0,
            .duty       = 0,
            .hpoint     = 0
        };

        return ledc_channel_config(&channelConf) == ESP_OK;
    }

    // Set servo angle in degrees (0–180)
    void write(int angle) {
        angle = constrain(angle, 0, 180);

        int pulseUs = map(angle, 0, 180, minUs, maxUs);

        // Convert microseconds → LEDC duty
        uint32_t maxDuty = (1 << 16) - 1;
        uint32_t duty = (pulseUs * maxDuty * freq) / 1000000;

        ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
    }

    // Optional: write directly in microseconds
    void writeMicroseconds(int us) {
        us = constrain(us, minUs, maxUs);

        uint32_t maxDuty = (1 << 16) - 1;
        uint32_t duty = (us * maxDuty * freq) / 1000000;

        ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
    }

private:
    int pin;
    ledc_channel_t channel;
    int minUs, maxUs;
    int freq;  // 50 Hz for standard RC servos
};

#endif
