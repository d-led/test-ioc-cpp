include 'premake'

make_solution 'test_ioc_cpp'

configuration { "vs*" }
	defines '_VARIADIC_MAX=10'
	buildoptions {
		"/bigobj"
	}

configuration { "macosx" }
	defines 'GTEST_USE_OWN_TR1_TUPLE=1'

configuration'*' --reset

platforms 'native'

local OS = os.get()
local settings = {
        links = {
                linux = { 'boost_system', 'pthread' },
                windows = { },
                macosx = { 'boost_system' }
        },
        libdirs = {
        		linux = {},
        		windows = {
        			path.join( os.getenv 'BOOST', 'lib32-msvc-12.0' )
        		},
        		macosx = {
        			'/usr/local/lib'
        		}
    	},
    	includedirs = {
    			linux = {},
    			windows = {
    				os.getenv 'BOOST'
    			},
    			macosx = {
                    '/usr/local/include'
                }
    	}
}

includedirs {
	'./googlemock/fused-src',
	'./Hypodermic',
	'./sauce',
	'./wallaroo',
	'./PocoCapsule/include',
	'./picojson',
	'./dicpp/include',
	settings.includedirs[OS]
}

make_static_lib("googlemock", {"./googlemock/fused-src/gmock-gtest-all.cc"} )
use_standard 'c++0x'

make_static_lib("googlemock-main", {"./googlemock/fused-src/gmock_main.cc"} )
use_standard 'c++0x'

make_static_lib("dicpp", {"./dicpp/lib/src/**.cpp"} )
use_standard 'c++0x'

-- make_static_lib("hypodermic", {"./hypodermic/Hypodermic/**.cpp"} )
-- use_standard 'c++0x'

make_console_app('test_ioc_cpp',
	{
		'./*.cpp',
		'./intrusive/*.cpp',
		'./common/implementations/*.cpp'
	}
)

links {
	'googlemock',
	'googlemock-main',
	'dicpp',
	-- 'hypodermic',
	settings.links[OS]
}

libdirs {
	settings.libdirs[OS]
}

defines {
	-- 'DI_DISABLE_LOGGING'
}

use_standard 'c++0x'

run_target_after_build()


newaction {
        trigger     = "prepare",
        description = "check out non-git dependencies",
        execute     = function ()
            os.execute [[hg clone https://bitbucket.org/cheez/dicpp]]
        end
}
