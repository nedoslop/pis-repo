#pragma once
#include <ostream>
#include <string>

struct MyTime {
    int hour;
    int minute;

    MyTime();
    MyTime(const std::string& data);
    bool is_valid();

    friend std::ostream& operator<<(std::ostream& os, const MyTime& self) {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "%02d:%02d", self.hour, self.minute);
        return os << buf;
    }
};