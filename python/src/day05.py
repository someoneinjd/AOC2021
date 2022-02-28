from timeit import timeit
from array import array

map = array("H", (0 for _ in range(1000_000)))

def solution1(data: str) -> int:
    for i in range(1000_000):
        map[i] = 0
    overlaps = 0
    for line in data.splitlines():
        p1, _, p2 = line.split()
        x1, y1, x2, y2 = (
            int(p1.split(",")[0]),
            int(p1.split(",")[1]),
            int(p2.split(",")[0]),
            int(p2.split(",")[1]),
        )
        x = min(x1, x2)
        y = min(y1, y2)
        xx = max(x1, x2)
        yy = max(y1, y2)
        if x == xx:
            for y in range(y, yy + 1):
                overlaps += map[x + y * 1000] == 1
                map[x + y * 1000] += 1
        elif y == yy:
            for x in range(x, xx + 1):
                overlaps += map[x + y * 1000] == 1
                map[x + y * 1000] += 1
    return overlaps


def solution2(data: str) -> int:
    for i in range(1000_000):
        map[i] = 0
    overlaps = 0
    for line in data.splitlines():
        p1, _, p2 = line.split()
        x, y, xx, yy = (
            int(p1.split(",")[0]),
            int(p1.split(",")[1]),
            int(p2.split(",")[0]),
            int(p2.split(",")[1]),
        )
        stepx = 0 if x == xx else 1 if xx > x else -1
        stepy = 0 if y == yy else 1 if yy > y else -1
        for _ in range(max(abs(xx - x), abs(yy - y)) + 1):
            overlaps += map[x + y * 1000] == 1
            map[x + y * 1000] += 1
            x += stepx
            y += stepy
    return overlaps


data = open("input/input05.txt").read()
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
