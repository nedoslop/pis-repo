#define _CRT_SECURE_NO_WARNINGS
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

    MyDate() : year(0), month(0), day(0) {}

    MyDate(const std::string& data) {
        if (std::sscanf(data.c_str(), "%d.%d.%d", &year, &month, &day) != 3)
            year = month = day = 0;
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
        if (std::sscanf(data.c_str(), "%d:%d", &hour, &minute) != 2)
            hour = minute = 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const MyTime& self) {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "%02d:%02d", self.hour, self.minute);
        return os << buf;
    }
};

class BasicObject {
public:
    virtual ~BasicObject() = default;
    virtual std::ostream& print(std::ostream& os) const { return os << "Incorrect basic object"; }

    friend std::ostream& operator<<(std::ostream& os, const BasicObject& self) {
        return self.print(os);
    }
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
        if (spl.size() < 1 || std::sscanf(spl[0].c_str(), "%d-%d", &floor, &cabinet) != 2)
            floor = cabinet = 0;
        name = spl.size() > 1 ? spl[1] : "";
    }

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

    std::ostream& print(std::ostream& os) const override {
        return os << "Lesson(teacher='" << teacher << "', name='" << name << "', date=" << date
                  << ", time=" << time << ")";
    }
};

static BasicObject* parse_object(std::string_view s) {
    if (s.starts_with("Lesson "))
        return new Lesson(s.substr(7));
    else if (s.starts_with("Location "))
        return new Location(s.substr(9));
    else if (s.starts_with("ClassRoom "))
        return new ClassRoom(s.substr(10));
    else
        return new BasicObject();
}

static std::vector<BasicObject*> parse_objects(const std::string& fp) {
    std::vector<BasicObject*> res;
    std::ifstream file(fp);
    if (!file.is_open())
        return res;

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            res.push_back(parse_object(line));
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
