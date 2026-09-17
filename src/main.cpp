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
