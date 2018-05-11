project('termdb', 'cpp', version : '0.10.0',
		default_options : ['cpp_std=c++11'])

inc = include_directories('include')

doctest = dependency('doctest')
optional = dependency('optional-lite')
variant = dependency('variant')

subdir('test')

if host_machine.system() == 'linux'
  extract_data = find_program('scripts/extract_data.sh')
elif host_machine.system() == 'windows'
  extract_data = find_program('scripts/extract_data.bat')
endif

extract_data = find_program('scripts/extract_data.sh')
run_target('extract_data', command : extract_data.path())

run_target('cppcheck', command : ['cppcheck', '--project=' +
  			join_paths(meson.build_root(), 'compile_commands.json')])
