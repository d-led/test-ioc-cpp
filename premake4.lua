_G.package.path=_G.package.path..[[;./?.lua;./?/?.lua]]

assert( require 'premake.quickstart' )

make_solution 'test_ioc_cpp'

includedirs {
	'./Catch/single_include',
	'./Hypodermic',
	'./sauce',
	'./wallaroo',
	'./PocoCapsule/include'
}

make_console_app('test_ioc_cpp', { './*.cpp' })

make_cpp11()

run_target_after_build()
