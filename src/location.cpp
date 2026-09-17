#include "location.hpp"
#include "utils.hpp"

Location::Location(std::string_view street, int num) : street(street), num(num) {}

Location::Location(std::string_view data) {
    auto spl = utils::split_string(data);
    street = spl.size() > 0 ? spl[0] : "";
    num = spl.size() > 1 ? utils::try_parse_int(spl[1]).value_or(0) : 0;
}

bool Location::is_valid() { return !street.empty() && num > 0; }

std::ostream& Location::print(std::ostream& os) const {
    return os << "Location(street='" << street << "', num=" << num << ")";
}