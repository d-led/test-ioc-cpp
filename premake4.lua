_G.package.path=_G.package.path..[[;./?.lua;./?/?.lua]]

assert( require 'premake.quickstart' )

make_solution 'test_ioc_cpp'

make_console_app('test_ioc_cpp', { 'test_ioc_cpp.cpp' })

make_cpp11()

