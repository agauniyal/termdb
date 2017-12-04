mainTest = executable('mainTest', 'test.cpp', include_directories : inc,
        dependencies : [doctest, optional, variant])
test('mainTest', mainTest)

stressTest = executable('stressTest', 'stressTest.cpp',
        include_directories : inc, dependencies : [optional, variant])
test('stressTest', stressTest)

bench = executable('bench', 'bench.cpp',
        include_directories : inc, dependencies : [optional, variant])
test('bench', bench)
