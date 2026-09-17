#pragma once
#include <ostream>
#include <string>

struct MyDate {
    int year;
    int month;
    int day;

    MyDate();
    MyDate(const std::string& data);
    bool is_valid();

    inline bool operator==(const MyDate& other) const {
        return year == other.year && month == other.month && day == other.day;
    }

    friend std::ostream& operator<<(std::ostream& os, const MyDate& self) {
        char buf[16];
        std::snprintf(buf, sizeof(buf), "%04d.%02d.%02d", self.year, self.month, self.day);
        return os << buf;
    }
};