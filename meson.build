project('termdb', 'cpp', version : '0.1.0',
		default_options : ['cpp_std=c++11'])

inc = include_directories('include')

thread_dep = dependency('threads')

gtest = dependency('gtest', main : true, required : false)

if not gtest.found()
    gtest_proj = subproject('gtest')
    gtest_inc = gtest_proj.get_variable('incdir')
    gtest_proj.get_variable('incdir')
    gtest_lib = static_library('gtest', gtest_proj.get_variable('libsources'),
                                gtest_proj.get_variable('mainsources'),
                                include_directories : gtest_inc)

    gtest = declare_dependency(include_directories : gtest_inc,
                               link_with : gtest_lib, dependencies: thread_dep)
endif

subdir('test')

extract_data = find_program('scripts/extract_data.sh')
run_target('extract_data', command : extract_data.path())
