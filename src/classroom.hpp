#pragma once
#include <string_view>
#include "basic_object.hpp"

class ClassRoom final : public BasicObject {
public:
    std::string name;
    int floor;
    int cabinet;

    ClassRoom(std::string_view name, int floor, int cabinet);
    ClassRoom(std::string_view data);
    bool is_valid();

private:
    std::ostream& print(std::ostream& os) const override;
};
