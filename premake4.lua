_G.package.path=_G.package.path..[[;./?.lua;./?/?.lua]]

assert( require 'premake.quickstart' )

make_solution 'test_ioc_cpp'

includedirs {
	'./googlemock/fused-src',
	'./Hypodermic',
	'./sauce',
	'./wallaroo',
	'./PocoCapsule/include'
}

make_static_lib("googlemock", {"./googlemock/fused-src/gmock-gtest-all.cc"} )

make_static_lib("googlemock-main", {"./googlemock/fused-src/gmock_main.cc"} )

make_console_app('test_ioc_cpp', { './*.cpp' })

links {
	'googlemock',
	'googlemock-main'
}

make_cpp11()

run_target_after_build()
