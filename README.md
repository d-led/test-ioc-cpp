test ioc c++
============

hello world c++ ioc containers

[![Build Status](https://travis-ci.org/d-led/test-ioc-cpp.png?branch=master)](https://travis-ci.org/d-led/test-ioc-cpp)

This project is a series of hello world examples with the intent of feature comparison of c++ Dependency Injection Frameworks.

The frameworks currently aspiring:

 - [wallaroo](https://code.google.com/p/wallaroo/)
 - [sauce](https://github.com/phs/sauce)
 - [~~hypodermic~~](https://code.google.com/p/hypodermic) (currently doesn't build with clang-500.2.79 on MacOS)
 - [pococapsule](http://www.pocomatic.com/docs/whitepapers/pococapsule-cpp/)
 - [dicpp](https://bitbucket.org/cheez/dicpp/wiki/Home)
 - [di](https://github.com/krzysztof-jusiak/di)
 - [infectorpp](https://code.google.com/p/infectorpp/)

Build files are generated using the convenience wrapper for [premake](http://industriousone.com/premake) - [premake-meta-cpp](https://github.com/d-led/premake-meta-cpp)

Testing and mocking of C++ objects is done via [googlemock+googletest](https://code.google.com/p/googlemock/).

[Travis CI](https://travis-ci.org/) is used for a proof of concept continuous integration.

Preparing build
---------------

 - `premake\premake4 prepare` to check out non-git dependencies
 - `premake\premake4 gmake` or `premake\premake4 vs2012`+convert to `vc12` or `premake\premake4 xcode4` to generate build files
 - If you don't have Mercurial, download and extract [dicpp](https://bitbucket.org/cheez/dicpp/overview) into the dicpp folder
 
Blog
----

 - [Part 1, Wallaroo Introduction](http://ledentsov.de/2013/12/26/quest-for-a-cpp-dependency-injection-container-library-part-1-wallaroo-introduction/)
 - [Part 2, Some Sauce](http://ledentsov.de/2014/01/01/quest-for-dependency-injection-library-part2-some-sauce/)
 - [Part 3, Beginning dicpp](http://ledentsov.de/2014/01/02/cpp-dependency-injection-part3-dicpp-example/)
