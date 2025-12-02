#pragma once

struct ClockTime{
    int hour;
    int minute;
};
void syncNTP();
void initClock();
ClockTime getClockTime();
