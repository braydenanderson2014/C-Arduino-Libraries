#ifndef HOST_ARDUINO_SIM_RTCLIB_H
#define HOST_ARDUINO_SIM_RTCLIB_H

#include "Arduino.h"

class DateTime {
public:
    DateTime()
        : _year(1970), _month(1), _day(1), _hour(0), _minute(0), _second(0) {}

    DateTime(int year, int month, int day, int hour, int minute, int second)
        : _year(year), _month(month), _day(day), _hour(hour), _minute(minute), _second(second) {}

    unsigned long unixtime() const {
        // Lightweight monotonic approximation for host simulation.
        return millis() / 1000UL;
    }

    String timestamp() const {
        return String(_year) + "-" + String(_month) + "-" + String(_day) + " " +
               String(_hour) + ":" + String(_minute) + ":" + String(_second);
    }

private:
    int _year;
    int _month;
    int _day;
    int _hour;
    int _minute;
    int _second;
};

class RTC_DS3231 {
public:
    bool begin() {
        return true;
    }

    DateTime now() const {
        return _current;
    }

    void adjust(const DateTime& value) {
        _current = value;
    }

private:
    DateTime _current;
};

#endif
