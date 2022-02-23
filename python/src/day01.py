from time import process_time_ns


def solution1(data: str) -> int:
    lines = [int(i) for i in data.splitlines()]
    return sum(b > a for a, b in zip(lines, lines[1:]))


def solution2(data: str) -> int:
    lines = [int(i) for i in data.splitlines()]
    return sum(b > a for a, b in zip(lines, lines[3:]))


data = open("input/input01.txt").read()
point1 = process_time_ns()
res1 = solution1(data)
point2 = process_time_ns()
res2 = solution2(data)
point3 = process_time_ns()
print(
    "{}\n{}\nElapsed Time 1:\n\t{}ms\nElasped Time 2:\n\t{}ms".format(
        res1,
        res2,
        (point2 - point1) / 1000000.0,
        (point3 - point2) / 1000000.0,
    )
)
