add_requires('range-v3')
set_languages('c++20')
add_rules('mode.asan')

option('benchmark')
    set_default(false)
    set_showmenu(true)
    add_defines('BENCHMARK')
option_end()

if is_mode('release') then
    add_cxxflags('-O2')
end

local day = {
    'day01', 'day02', 'day03', 'day04',
    'day05', 'day06', 'day07', 'day08',
    'day09', 'day10', 'day11', 'day12',
    'day13', 'day14', 'day15', 'day16',
    'day17', 'day18', 'day19', 'day20',
    'day21', 'day22', 'day23', 'day24',
    'day25'
}

for _, name in ipairs(day) do 
    target(name)
        set_kind('binary')
        add_files(path.join('src', name, 'main.cpp'))
        add_packages('range-v3')
        add_options('benchmark')
    target_end()
end
