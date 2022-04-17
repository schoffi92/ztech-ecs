cmake_minimum_required( VERSION "3.7.1" )

set( VERSION 1.0.0 )

project( "ZTech" VERSION ${VERSION} )

set(CMAKE_CXX_STANDARD 17)

file( GLOB_RECURSE SRC "${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp" )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)

#add_library( "ztech.ecs" STATIC "${SRC}" )
add_executable( "ecstest" "${SRC}" )