#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <optional>

namespace utils {
std::vector<std::string> split_string(std::string_view data);

std::optional<int> try_parse_int(std::string_view s);
}