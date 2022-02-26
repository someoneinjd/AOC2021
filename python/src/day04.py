from timeit import timeit


def solution1(data: str) -> int:
    pos = data.find("\n\n")
    nums = data[:pos:]
    boards = data[pos + 2 : :]
    new_boards = [
        [{int(val): idx for idx, val in enumerate(line.split())}, 0]
        for line in boards.split("\n\n")
    ]
    for n in (int(i) for i in nums.split(",")):
        for b in new_boards:
            if n not in b[0]:
                continue
            b[1] |= 1 << b[0][n]
            if any(
                (b[1] >> (i * 5) & 0b11111) == 0b11111
                or (b[1] >> i & 0b100001000010000100001) == 0b100001000010000100001
                for i in range(5)
            ):
                return sum((b[1] >> i & 1 ^ 1) * fst * n for fst, i in b[0].items())
    return -1


def solution2(data: str) -> int:
    pos = data.find("\n\n")
    new_boards = [
        [{int(val): idx for idx, val in enumerate(line.split())}, 0]
        for line in data[pos + 2 : :].split("\n\n")
    ]
    mark, num = 0, 0
    board: dict[int, int] = {}
    for n in (int(i) for i in data[:pos:].split(",")):
        for b in new_boards:
            if n not in b[0]:
                continue
            b[1] |= 1 << b[0][n]
            if any(
                (b[1] >> (i * 5) & 0b11111) == 0b11111
                or (b[1] >> i & 0b100001000010000100001) == 0b100001000010000100001
                for i in range(5)
            ):
                mark = b[1]
                num = n
                board = b[0]
                b[0] = {}
    return sum((mark >> i & 1 ^ 1) * n * num for n, i in board.items())


data = open("input/input04.txt").read()
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
