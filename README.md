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

Build files are generated using the convenience wrapper for [premake](http://industriousone.com/premake) - [premake-meta-cpp](https://github.com/d-led/premake-meta-cpp)

Testing and mocking of C++ objects is done via [googlemock+googletest](https://code.google.com/p/googlemock/).

[Travis CI](https://travis-ci.org/) is used for a proof of concept continuous integration.
 
The blog entries will be linked here
