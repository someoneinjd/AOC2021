from timeit import timeit


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
res1 = solution1(data)
res2 = solution2(data)
print(
    "{}\n{}\nElapsed Time 1:\n\t{:.6f}ms\nElasped Time 2:\n\t{:.6f}ms".format(
        res1,
        res2,
        timeit("solution1(data)", number=100, globals=globals()) * 10.0,
        timeit("solution2(data)", number=100, globals=globals()) * 10.0,
    )
)
