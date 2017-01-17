test_sources = ['test.cpp']

if meson.get_compiler('cpp').get_id() == 'clang'
  extra_args = ['--coverage']
else
  extra_args = []
endif

testexe = executable('testexe', test_sources, include_directories : inc,
        dependencies : [gtest], link_args : extra_args)

test('app test', testexe, args: '--gtest_color=yes')
