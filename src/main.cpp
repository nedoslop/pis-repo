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
#include "basic_object.hpp"
#include "location.hpp"
#include "classroom.hpp"
#include "lesson.hpp"

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

#if 0
#include <UnitTest++/UnitTest++.h>

TEST(MyDateTest) {
    MyDate date1("2024.08.11");
    CHECK_EQUAL(true, date1.is_valid());
    CHECK_EQUAL(11, date1.day);
    CHECK_EQUAL(8, date1.month);
    CHECK_EQUAL(2024, date1.year);

    MyDate date2("2024.08.34");
    CHECK_EQUAL(false, date2.is_valid());

    MyDate date3(" gfdgdfgdfg 1241.08.34");
    CHECK_EQUAL(false, date3.is_valid());
}

TEST(MyTimeTest) {
    MyTime time1("11:52");
    CHECK_EQUAL(true, time1.is_valid());
    CHECK_EQUAL(11, time1.hour);
    CHECK_EQUAL(52, time1.minute);

    MyTime time2("11:67");
    CHECK_EQUAL(false, time2.is_valid());

    MyTime time3("2024.08.34");
    CHECK_EQUAL(false, time3.is_valid());
}

TEST(ClassRoomTest) {
    ClassRoom cl1("2-13 \"class name 2\"");
    CHECK_EQUAL(true, cl1.is_valid());
    CHECK_EQUAL(2, cl1.floor);
    CHECK_EQUAL(13, cl1.cabinet);
    CHECK_EQUAL("class name 2", cl1.name);

    ClassRoom cl2("-2-13 \"class name 2\"");
    CHECK_EQUAL(false, cl2.is_valid());

    ClassRoom cl3("2-13");
    CHECK_EQUAL(false, cl3.is_valid());
}

TEST(LessonTest) {
    Lesson l1("2010.12.01 13:45 \"Alexey Y\" \"class name 1\"");
    CHECK_EQUAL(true, l1.is_valid());
    CHECK_EQUAL(MyDate("2010.12.01"), l1.date);
    CHECK_EQUAL(MyTime("13:45"), l1.time);
    CHECK_EQUAL(MyTime("Alexey Y"), l1.teacher);
    CHECK_EQUAL(MyTime("class name 1"), l1.name);

    Lesson l2("22144.12.01 13:45 \"Alexey Y\" \"class name 1\"");
    CHECK_EQUAL(false, l2.is_valid());

    Lesson l3("13:45 \"Alexey Y\" \"class name 1\"");
    CHECK_EQUAL(false, l3.is_valid());
}

TEST(LocationTest) {
    Location l1("\"Some location\" 13");
    CHECK_EQUAL(true, l1.is_valid());
    CHECK_EQUAL("Some location", l1.street);
    CHECK_EQUAL(13, l1.num);

    Location l2("\"Some location\" 0");
    CHECK_EQUAL(false, l2.is_valid());

    Location l3("123");
    CHECK_EQUAL(false, l3.is_valid());

    Location l4("\"test loc\"");
    CHECK_EQUAL(false, l4.is_valid());
}

int main(int, const char*[]) { return UnitTest::RunAllTests(); }

#else
int main(int argc, char* argv[]) {
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
#endif
