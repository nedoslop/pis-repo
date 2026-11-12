#pragma once
#include <vector>
#include <string>
#include <string_view>
#include <optional>

namespace utils {
/**
 * @brief Split string by spaces
 * @param data Input string.
 * @return Output vector of strings.
 */
std::vector<std::string> split_string(std::string_view data);

/**
 * @brief Try parsing int from string
 * @param data Input string.
 * @return Parse result or std::nullopt.
 */
std::optional<int> try_parse_int(std::string_view s);
}