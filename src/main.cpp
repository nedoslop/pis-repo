#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <charconv>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

static std::vector<std::string> split_string(std::string_view data) {
    std::vector<std::string> ret;
    std::string cur;
    bool is_str = false;
    for (char c : data) {
        if (c == '"') {
            is_str = !is_str;
            continue;
        } else if (c == ' ' && !is_str) {
            if (!cur.empty()) {
                ret.push_back(cur);
                cur.clear();
            }
            continue;
        }
        cur += c;
    }
    if (!cur.empty())
        ret.push_back(cur);
    return ret;
}

static std::optional<int> try_parse_int(std::string_view s) {
    int value = 0;
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);

    if (ec == std::errc{} && ptr == s.data() + s.size())
        return value;

    return std::nullopt;
}

struct MyDate {
    int year;
    int month;
    int day;

    static int max_days_in_month(int month) {
        switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 2:
            return 29;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 0;
        }
    }

    MyDate() : year(0), month(0), day(0) {}

    MyDate(const std::string& data) {
        if (std::sscanf(data.c_str(), "%d.%d.%d", &year, &month, &day) != 3 || !is_valid())
            year = month = day = -1;
    }

    bool is_valid() {
        return year >= 2000 && year < 2100 && month >= 1 && month <= 12 && day >= 1 &&
               day <= max_days_in_month(month);
    }

    friend std::ostream& operator<<(std::ostream& os, const MyDate& self) {
        char buf[16];
        std::snprintf(buf, sizeof(buf), "%04d.%02d.%02d", self.year, self.month, self.day);
        return os << buf;
    }
};

struct MyTime {
    int hour;
    int minute;

    MyTime() : hour(0), minute(0) {}

    MyTime(const std::string& data) {
        if (std::sscanf(data.c_str(), "%d:%d", &hour, &minute) != 2 || !is_valid())
            hour = minute = -1;
    }

    bool is_valid() { return hour >= 0 && hour < 24 && minute >= 0 && minute < 60; }

    friend std::ostream& operator<<(std::ostream& os, const MyTime& self) {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "%02d:%02d", self.hour, self.minute);
        return os << buf;
    }
};

class BasicObject {
public:
    virtual ~BasicObject() = default;

    friend std::ostream& operator<<(std::ostream& os, const BasicObject& self) {
        return self.print(os);
    }

private:
    virtual std::ostream& print(std::ostream& os) const = 0;
};

class Location final : public BasicObject {
public:
    std::string street;
    int num;

    Location(std::string_view street, int num) : street(street), num(num) {}
    Location(std::string_view data) {
        auto spl = split_string(data);
        street = spl.size() > 0 ? spl[0] : "";
        num = spl.size() > 1 ? try_parse_int(spl[1]).value_or(0) : 0;
    }

    bool is_valid() { return !street.empty() && num > 0; }

private:
    std::ostream& print(std::ostream& os) const override {
        return os << "Location(street='" << street << "', num=" << num << ")";
    }
};

class ClassRoom final : public BasicObject {
public:
    std::string name;
    int floor;
    int cabinet;

    ClassRoom(std::string_view name, int floor, int cabinet)
        : name(name), floor(floor), cabinet(cabinet) {}
    ClassRoom(std::string_view data) {
        auto spl = split_string(data);
        name = spl.size() > 1 ? spl[1] : "";
        if (spl.size() < 1 || std::sscanf(spl[0].c_str(), "%d-%d", &floor, &cabinet) != 2 ||
            !is_valid())
            floor = cabinet = 0;
    }

    bool is_valid() { return !name.empty() && floor > 0 && cabinet > 0; }

private:
    std::ostream& print(std::ostream& os) const override {
        return os << "ClassRoom(name='" << name << "', floor=" << floor << ", cabinet=" << cabinet
                  << ")";
    }
};

class Lesson final : public BasicObject {
public:
    std::string teacher;
    std::string name;
    MyDate date;
    MyTime time;

    Lesson(std::string_view teacher, std::string_view name, MyDate date, MyTime time)
        : teacher(teacher), name(name), date(date), time(time) {}
    Lesson(std::string_view data) {
        auto spl = split_string(data);
        date = spl.size() > 0 ? MyDate(spl[0]) : MyDate();
        time = spl.size() > 1 ? MyTime(spl[1]) : MyTime();
        teacher = spl.size() > 2 ? spl[2] : "";
        name = spl.size() > 3 ? spl[3] : "";
    }

    bool is_valid() {
        return date.is_valid() && time.is_valid() && !teacher.empty() && !name.empty();
    }

private:
    std::ostream& print(std::ostream& os) const override {
        return os << "Lesson(teacher='" << teacher << "', name='" << name << "', date=" << date
                  << ", time=" << time << ")";
    }
};

template <typename T> static std::optional<BasicObject*> filter_valid_object(T* obj) {
    if (obj->is_valid())
        return obj;
    delete obj;
    return std::nullopt;
}

static std::optional<BasicObject*> parse_object(std::string_view s) {
    if (s.starts_with("Lesson ")) {
        return filter_valid_object(new Lesson(s.substr(7)));
    } else if (s.starts_with("Location ")) {
        return filter_valid_object(new Location(s.substr(9)));
    } else if (s.starts_with("ClassRoom "))
        return filter_valid_object(new ClassRoom(s.substr(10)));
    else
        return std::nullopt;
}

static std::vector<BasicObject*> parse_objects(const std::string& fp) {
    std::vector<BasicObject*> res;
    std::ifstream file(fp);
    if (!file.is_open())
        return res;

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            auto obj = parse_object(line);
            if (obj.has_value())
                res.push_back(obj.value());
        }
    }

    return res;
}

static std::vector<std::string> get_unused_names(const std::vector<BasicObject*>& arr) {
    std::vector<std::string> used;
    std::vector<std::string> ret;

    for (const BasicObject* obj : arr) {
        if (auto lesson = dynamic_cast<const Lesson*>(obj)) {
            if (std::find(used.begin(), used.end(), lesson->name) == used.end())
                used.push_back(lesson->name);
        }
    }

    for (const BasicObject* obj : arr) {
        if (auto classroom = dynamic_cast<const ClassRoom*>(obj)) {
            if (std::find(used.begin(), used.end(), classroom->name) == used.end() &&
                std::find(ret.begin(), ret.end(), classroom->name) == ret.end())
                ret.push_back(classroom->name);
        }
    }

    return ret;
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    auto vec = parse_objects("input.txt");

    std::cout << "Parse result: " << std::endl;
    for (const BasicObject* i : vec)
        std::cout << *i << std::endl;
    std::cout << std::endl;

    std::cout << std::endl << "Unused: " << std::endl;
    for (const std::string& name : get_unused_names(vec))
        std::cout << name << std::endl;

    for (BasicObject* i : vec)
        delete i;

    std::system("pause");
    return 0;
}
