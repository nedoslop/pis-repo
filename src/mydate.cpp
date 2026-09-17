#define _CRT_SECURE_NO_WARNINGS
#include "mydate.hpp"

static int max_days_in_month(int year, int month) {
    switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    case 2:
        // Check if year is leap
        return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 29 : 28;
    default:
        return 0;
    }
}

MyDate::MyDate() : year(0), month(0), day(0) {}

MyDate::MyDate(const std::string& data) {
    if (std::sscanf(data.c_str(), "%d.%d.%d", &year, &month, &day) != 3 || !is_valid())
        year = month = day = -1;
}

bool MyDate::is_valid() {
    return year >= 2000 && year < 2100 && month >= 1 && month <= 12 && day >= 1 &&
           day <= max_days_in_month(year, month);
}