from datetime import datetime

def parse_date_time(date_time_string: str):
    return datetime.strptime(date_time_string, "%Y.%m.%d %H:%M")

def parse_str(str_string: str):
    return str_string[1:-1]

class Lesson:
    def __init__(self, teacher, time):
        self.teacher = teacher
        self.time = time

def main():
    print('Examples: ')
    print('1985.12.01 13:45 "Alexey"')
    print('  2018.01.30 15:45 "Vadim"')
    print('2005.06.06 16:10    "Alex andr"')
    print('\nEnter input string: ')
    user_text_arr = [x for x in input().strip().split() if x]
    lesson = Lesson(parse_str(' '.join(user_text_arr[2:])), parse_date_time(user_text_arr[0] + ' ' + user_text_arr[1]))
    print('\nResult:', lesson.teacher, lesson.time)


if __name__ == '__main__':
    main()
