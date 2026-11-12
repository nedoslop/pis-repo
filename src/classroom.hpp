#pragma once
#include <string_view>
#include "basic_object.hpp"

/**
 * @class ClassRoom
 * @brief Class which keeps name, floor and cabinet
 */
class ClassRoom final : public BasicObject {
public:
    std::string name;
    int floor;
    int cabinet;

    ClassRoom(std::string_view name, int floor, int cabinet);
    ClassRoom(std::string_view data);
    bool is_valid() const;

private:
    std::ostream& print(std::ostream& os) const override;
};
