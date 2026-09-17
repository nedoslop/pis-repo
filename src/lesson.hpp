#include <string>
#include <ostream>
#include "basic_object.hpp"
#include "mydate.hpp"
#include "mytime.hpp"

class Lesson final : public BasicObject {
public:
    std::string teacher;
    std::string name;
    MyDate date;
    MyTime time;

    Lesson(std::string_view teacher, std::string_view name, MyDate date, MyTime time);
    Lesson(std::string_view data);
    bool is_valid();

private:
    std::ostream& print(std::ostream& os) const override;
};