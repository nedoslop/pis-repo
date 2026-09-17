#include "lesson.hpp"
#include "utils.hpp"

Lesson::Lesson(std::string_view teacher, std::string_view name, MyDate date, MyTime time)
    : teacher(teacher), name(name), date(date), time(time) {}
Lesson::Lesson(std::string_view data) {
    auto spl = utils::split_string(data);
    date = spl.size() > 0 ? MyDate(spl[0]) : MyDate();
    time = spl.size() > 1 ? MyTime(spl[1]) : MyTime();
    teacher = spl.size() > 2 ? spl[2] : "";
    name = spl.size() > 3 ? spl[3] : "";
}

bool Lesson::is_valid() {
    return date.is_valid() && time.is_valid() && !teacher.empty() && !name.empty();
}

std::ostream& Lesson::print(std::ostream& os) const {
    return os << "Lesson(teacher='" << teacher << "', name='" << name << "', date=" << date
              << ", time=" << time << ")";
}
