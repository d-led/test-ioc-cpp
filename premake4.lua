_G.package.path=_G.package.path..[[;./?.lua;./?/?.lua]]

assert( require 'premake.quickstart' )

make_solution 'test_ioc_cpp'

includedirs {
	'./googlemock/fused-src',
	'./Hypodermic',
	'./sauce',
	'./wallaroo',
	'./PocoCapsule/include',
	'./picojson'
}

make_static_lib("googlemock", {"./googlemock/fused-src/gmock-gtest-all.cc"} )

make_static_lib("googlemock-main", {"./googlemock/fused-src/gmock_main.cc"} )

make_console_app('test_ioc_cpp',
	{
		'./*.cpp',
		'./intrusive/*.cpp',
		'./common/implementations/*.cpp'
	}
)

links {
	'googlemock',
	'googlemock-main'
}

make_cpp11()

run_target_after_build()


newaction {
        trigger     = "prepare",
        description = "check out non-git dependencies",
        execute     = function ()
            os.execute [[hg clone https://bitbucket.org/cheez/dicpp]]
        end
}
