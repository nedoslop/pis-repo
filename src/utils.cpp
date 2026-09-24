#include "utils.hpp"
#include <charconv>

namespace utils {
std::vector<std::string> split_string(std::string_view data) {
    std::vector<std::string> ret;
    std::string cur;
    bool is_str = false;
    for (char c : data) {
        if (c == '"') {
            is_str = !is_str;
            continue;
        } else if (c == ' ' && !is_str) {
            if (!cur.empty()) {
                ret.push_back(cur);
                cur.clear();
            }
            continue;
        }
        cur += c;
    }
    if (!cur.empty())
        ret.push_back(cur);
    return ret;
}

std::optional<int> try_parse_int(std::string_view s) {
    int value = 0;
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value);

    if (ec == std::errc{} && ptr == s.data() + s.size())
        return value;

    return std::nullopt;
}

std::string join_string_vec(std::vector<std::string> vec) {
    if (vec.empty())
        return "";
    std::string ret;
    auto it = vec.begin();
    ret += *it;
    it++;
    for (; it != vec.end(); it++) {
        ret += ", ";
        ret += *it;
    }
    return ret;
}
} // namespace utils