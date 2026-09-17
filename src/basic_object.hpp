#pragma once
#include <ostream>

class BasicObject {
public:
    virtual ~BasicObject() = default;

    friend std::ostream& operator<<(std::ostream& os, const BasicObject& self) {
        return self.print(os);
    }

private:
    virtual std::ostream& print(std::ostream& os) const = 0;
};
