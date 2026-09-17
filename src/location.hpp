#pragma once
#include "basic_object.hpp"

class Location final : public BasicObject {
public:
    std::string street;
    int num;

    Location(std::string_view street, int num);
    Location(std::string_view data);
    bool is_valid();

private:
    std::ostream& print(std::ostream& os) const override;
};