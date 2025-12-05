#pragma once

struct ClockTime{
    int hour;
    int minute;
    int weekDay;
};
extern ClockTime timeNow;
void syncNTP();
void initClock();
ClockTime getClockTime();
