#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include "utils.hpp"
#include "mydate.hpp"
#include "mytime.hpp"

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
        auto spl = utils::split_string(data);
        street = spl.size() > 0 ? spl[0] : "";
        num = spl.size() > 1 ? utils::try_parse_int(spl[1]).value_or(0) : 0;
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
        auto spl = utils::split_string(data);
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
        auto spl = utils::split_string(data);
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
