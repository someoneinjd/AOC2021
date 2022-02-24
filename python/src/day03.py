from timeit import timeit


def solution1(data: str) -> int:
    tmp = [0 for _ in range(12)]
    for bits in (int(line, 2) for line in data.splitlines()):
        tmp = [val + ((bits & 1 << idx) >> idx) for idx, val in enumerate(tmp)]
    res = sum((val >= 500) << idx for idx, val in enumerate(tmp))
    return res * (~res & ((1 << 12) - 1))


def solution2(data: str) -> int:
    num1 = [int(line, 2) for line in data.splitlines()]
    num2 = num1.copy()
    res1, res2 = 0, 0
    for i in range(11, -1, -1):
        one1 = sum((n & (1 << i)) > 0 for n in num1) >= (len(num1) + 1) // 2
        one2 = sum((n & (1 << i)) > 0 for n in num2) >= (len(num2) + 1) // 2
        num1 = [n for n in num1 if ((n & (1 << i)) > 0) != one1]
        num2 = [n for n in num2 if ((n & (1 << i)) > 0) == one2]
        if len(num1) == 1:
            res1 = num1[0]
        if len(num2) == 1:
            res2 = num2[0]
    return res1 * res2


data = open("input/input03.txt").read()
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
