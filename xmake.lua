package('range-v3')

    set_homepage('https://github.com/ericniebler/range-v3/')
    set_description('Range library for C++14/17/20, basis for C++20\'s std::ranges')
    set_license('BSL-1.0')

    add_urls('https://github.com/ericniebler/range-v3.git')

    add_deps('cmake')
    if is_plat('windows') then
        add_cxxflags('/permissive-')
    end

    on_install(function (package)
        local configs = {'-DRANGE_V3_DOCS=OFF', '-DRANGE_V3_TESTS=OFF', '-DRANGE_V3_EXAMPLES=OFF', '-DRANGE_V3_PERF=OFF'}
        table.insert(configs, '-DCMAKE_BUILD_TYPE=' .. (package:debug() and 'Debug' or 'Release'))
        import('package.tools.cmake').install(package, configs)
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            void test() {
                using namespace ranges;
                auto triples = views::for_each(views::iota(1), [](int z) {
                    return views::for_each(views::iota(1, z + 1), [=](int x) {
                        return views::for_each(views::iota(x, z + 1), [=](int y) {
                            return yield_if(x * x + y * y == z * z,
                                            std::make_tuple(x, y, z));
                        });
                    });
                });
            }
        ]]}, {configs = {languages = 'c++17'}, includes = 'range/v3/all.hpp'}))
    end)
package_end()


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
    'day17', 'day18', 'day20'
}

for _, name in ipairs(day) do 
    target(name)
        set_kind('binary')
        add_files('src/' .. name .. '/main.cpp')
        add_packages('range-v3')
        add_options('benchmark')
    target_end()
end
