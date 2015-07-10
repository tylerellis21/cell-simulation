project (cell-simulation)
cmake_minimum_required (VERSION 2.6)

# Setup build type to be debug by default. Can be changed on cli: -DCMAKE_BUILD_TYPE=Release.
if (NOT DEFINED CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif()

# General settings
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_COLOR_MAKEFILE ON)

# Set some SFML build settings.
set (SFML_BUILD_DOC false)
set (SFML_BUILD_EXAMPLES false)
set (BUILD_SHARED_LIBS false)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Enable C++11
#if (APPLE)
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -stdlib=libc++")
#elseif (UNIX)
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
#elseif (WIN32)#
#
#    if (MINGW)
#        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
#    endif()
	
#endif(APPLE)

# Build static stdlibs on GCC and MingW
if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

    if (WIN32)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-Bstatic -lstdc++ -lpthread")
    endif(WIN32)
endif()

#if (${CMAKE_BUILD_TYPE} EQUAL "Release")
#    set (${CMAKE_CXX_FLAGS} "-O")
#endif()

set (SFML_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/lib/sfml/include")
set (SFML_LIBS 
    sfml-network
    sfml-audio 
    sfml-graphics
    sfml-window
    sfml-system
)

set (NEX_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/lib/nex/include")
set (NEX_LIBS
    nex-math
    nex-gfx
)

# Set the exe to be in the proper folder.
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_subdirectory (lib/sfml)
add_subdirectory (lib/nex)
add_subdirectory (src)

