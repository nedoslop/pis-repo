#define _CRT_SECURE_NO_WARNINGS
#include "classroom.hpp"
#include "utils.hpp"
#include <string>

ClassRoom::ClassRoom(std::string_view name, int floor, int cabinet)
    : name(name), floor(floor), cabinet(cabinet) {}

ClassRoom::ClassRoom(std::string_view data) {
    auto spl = utils::split_string(data);
    name = spl.size() > 1 ? spl[1] : "";
    if (spl.size() < 1 || std::sscanf(spl[0].c_str(), "%d-%d", &floor, &cabinet) != 2 ||
        !is_valid())
        floor = cabinet = 0;
}

bool ClassRoom::is_valid() { return !name.empty() && floor > 0 && cabinet > 0; }

std::ostream& ClassRoom::print(std::ostream& os) const {
    return os << "ClassRoom(name='" << name << "', floor=" << floor << ", cabinet=" << cabinet
              << ")";
}