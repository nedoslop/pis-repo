#define _CRT_SECURE_NO_WARNINGS
#include "mytime.hpp"

MyTime::MyTime() : hour(0), minute(0) {}

MyTime::MyTime(const std::string& data) {
    if (std::sscanf(data.c_str(), "%d:%d", &hour, &minute) != 2 || !is_valid())
        hour = minute = -1;
}

bool MyTime::is_valid() { return hour >= 0 && hour < 24 && minute >= 0 && minute < 60; }
