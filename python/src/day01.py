from timeit import timeit


def solution1(data: str) -> int:
    lines = [int(i) for i in data.splitlines()]
    return sum(b > a for a, b in zip(lines, lines[1:]))


def solution2(data: str) -> int:
    lines = [int(i) for i in data.splitlines()]
    return sum(b > a for a, b in zip(lines, lines[3:]))


data = open("input/input01.txt").read()
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
