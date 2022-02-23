from time import process_time_ns


def solution1(data: str) -> int:
    f, d = 0, 0
    for line in data.splitlines():
        action, step = line.split()
        match action:
            case "forward":
                f += int(step)
            case "down":
                d += int(step)
            case _:
                d -= int(step)

    return f * d


def solution2(data: str) -> int:
    f, d, a = 0, 0, 0
    for line in data.splitlines():
        action, step = line.split()
        match action:
            case "forward":
                num = int(step)
                f += num
                d += num * a
            case "down":
                a += int(step)
            case _:
                a -= int(step)

    return f * d


data = open("input/input02.txt").read()
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
