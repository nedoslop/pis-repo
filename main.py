from datetime import datetime


def parse_date_time(date_time_string: str):
    """
    Convert date and time string into datetime class
    """
    return datetime.strptime(date_time_string, "%Y.%m.%d %H:%M")


def split_str(s: str) -> list:
    """
    Split input string into array (properply processing string literals)
    """
    ret = []
    cur = ""
    is_str = False
    for i in s:
        if i == '"':
            is_str = not is_str
            continue
        elif i == " " and not is_str:
            if cur.strip():
                ret.append(cur.strip())
            cur = ""
            continue
        cur += i
    if cur.strip():
        ret.append(cur.strip())
    return ret


class Location:
    def __init__(self, street: str, num: int):
        self.street = street
        self.num = num

    @staticmethod
    def from_str(s: str):
        spl = [x for x in s.replace('"', "").split() if x]
        return Location(" ".join(spl[:-1]).strip(), int(spl[-1]))

    def __str__(self):
        return f"Location(street='{self.street}', num={self.num})"


class ClassRoom:
    def __init__(self, floor: int, cabinet: int):
        self.floor = floor
        self.cabinet = cabinet

    @staticmethod
    def from_str(s: str):
        spl = s.split("-")
        return ClassRoom(int(spl[0]), int(spl[1]))

    def __str__(self):
        return f"ClassRoom(floor={self.floor}, cabinet={self.cabinet})"


class Lesson:
    def __init__(
        self, time: datetime, teacher: str, location: Location, classroom: ClassRoom
    ):
        self.time = time
        self.teacher = teacher
        self.location = location
        self.classroom = classroom

    def from_str(s: str):
        arr = split_str(s)
        return Lesson(
            parse_date_time(arr[0] + " " + arr[1]),
            arr[2],
            Location.from_str(arr[3]),
            ClassRoom.from_str(arr[4]),
        )

    def __str__(self):
        return f"Lesson(teacher='{self.teacher}', time={self.time}, location={self.location}, classroom={self.classroom})"


def parse_object(s: str) -> any:
    if s.startswith("Lesson "):
        return Lesson.from_str(s[7:])
    if s.startswith("Location "):
        return Location.from_str(s[9:])
    if s.startswith("ClassRoom "):
        return ClassRoom.from_str(s[10:])
    return None


def parse_objects(s: str) -> list:
    return [parse_object(x) for x in s.split("\n")]


def main():
    print('Format: YY.MM.DD hh:mm "Teacher" "Location" Floor-Cabinet')
    if False:
        print("Examples: ")
        print('1985.12.01 13:45 "Alexey Y" "Borisova 5" 4-12')
        print('  2018.01.30 15:45 "Vadim G" "Some_location 123" 4-56')
        print('2005.06.06 16:10    "Alex andr B" "location 456" 1-23')
        print("\nEnter input string: ")
        print("\nResult:", Lesson.from_str(input()))
    if True:
        arr = parse_objects(open("input.txt", encoding="utf-8").read())
        print("Parse result:")
        for i in arr:
            print(i)


if __name__ == "__main__":
    main()
