# AOC2021

C++/Python solutions for Advent of Code 2021 (using [range-v3](https://github.com/ericniebler/range-v3)) 

## Build && Run

```shell
# C++
# Use the xmake build system
# https://github.com/xmake-io/xmake
cd cpp
xmake && xmake run

# Python
cd python
find src/ -name '*.py' | xargs -i python3 {}
```

## Benchmark (Only for C++)

```shell
cd cpp
xmake f --benchmark=y
xmake && xmake run
```

## Debug (Only for C++)

```shell
cd cpp
xmake f -m asan
xmake && xmake run
```
