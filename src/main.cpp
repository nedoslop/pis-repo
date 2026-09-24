#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <map>
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
static std::pair<std::string, std::string> split_pair(std::string inp) {
    std::string a;
    std::string b;
    bool first = true;
    for (size_t i = 0; i < inp.size(); i++) {
        if (inp[i] == ' ')
            continue;
        if (inp[i] == '-' && (i + 1) < inp.size() && inp[i + 1] == '>')
            continue;
        if (i > 0 && inp[i] == '>' && inp[i - 1] == '-') {
            first = false;
            continue;
        }
        (first ? a : b) += inp[i];
    }
    // std::cout << "got " << a << "_" << b << std::endl;
    return {a, b};
}

void do_graph_task() {
    std::ifstream file("i2.txt");
    if (!file.is_open())
        return;

    std::string line;
    std::map<std::string, std::vector<std::string>> map;
    while (std::getline(file, line)) {
        auto [a, b] = split_pair(line);
        map[a].push_back(b);
    }

    std::cout << "YOUR INPUT:" << std::endl;
    for (const auto& [key, value] : map)
        std::cout << key << " -> [" << utils::join_string_vec(value) << "]" << std::endl;

    std::cout << "ENTER FROM:" << std::endl;
    std::string from;
    std::getline(std::cin, from);

    std::vector<std::string> q = {from};
    std::vector<std::string> possible;
    while (!q.empty()) {
        std::vector<std::string> next_q;
        for (std::string& f : q) {
            for (std::string& t : map[f]) {
                // std::cout << "TRYING " << f << " -> " << t << std::endl;
                if (std::find(possible.begin(), possible.end(), t) == possible.end()) {
                    possible.push_back(t);
                    next_q.push_back(t);
                }
            }
        }
        q = std::move(next_q);
    }

    std::cout << "POSSIBLE: " << utils::join_string_vec(possible) << std::endl;
    bool can = map.size() == possible.size();
    std::cout << "RESULT: " << (can ? "YES" : "NO") << std::endl;
}

int main(int argc, char* argv[]) {
    std::vector<BasicObject*> vec;

    do_graph_task();
    return 0;

    while (1) {
        std::system("cls");
        std::cout << "0. Exit " << std::endl;
        std::cout << "1. Add from str " << std::endl;
        std::cout << "2. Show " << std::endl;
        std::cout << "3. Add from file " << std::endl;
        std::cout << "Your choice: ";
        std::string choice;
        std::getline(std::cin, choice);
        if (choice.empty())
            continue;
        switch (choice[0]) {
        case '0': {
            std::cout << std::endl;
            return 0;
        }
        case '1': {
            std::cout << std::endl;
            std::cout << "Enter string: ";
            std::getline(std::cin, choice);
            if (auto opt = parse_object(choice))
                vec.push_back(opt.value());
            else
                std::cout << "ERROR!" << std::endl;
            std::system("pause");
            break;
        }
        case '2': {
            std::cout << std::endl;
            std::cout << "Parse result: " << std::endl;
            for (const BasicObject* i : vec)
                std::cout << *i << std::endl;
            std::cout << std::endl;

            std::cout << std::endl << "Unused: " << std::endl;
            for (const std::string& name : get_unused_names(vec))
                std::cout << name << std::endl;
            std::system("pause");
            break;
        }
        case '3': {
            auto need = parse_objects("input.txt");
            vec.insert(vec.end(), need.begin(), need.end());
            break;
        }
        }
    }

    for (BasicObject* i : vec)
        delete i;

    return 0;
}
#endif
