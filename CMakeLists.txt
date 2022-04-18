cmake_minimum_required( VERSION "3.7.1" )

set( VERSION 1.0.0 )

project( "ZTech" VERSION ${VERSION} )

set(CMAKE_CXX_STANDARD 17)

file( GLOB_RECURSE SRC "${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp" )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)

add_library( "ztech.ecs" STATIC "${SRC}" )
#add_executable( "ecstest" "${SRC}" )

if( BUILD_TESTING )
    MESSAGE( "Adding UnitTests" )
    add_executable( "ecstest" "unit_test/main.cpp" "${SRC}" )
    add_test( "UnitTest" "ecstest" )

    add_executable( "ecsreg" "unit_test/check_component_registration" "${SRC}" )
    add_test( "EcsReg" "ecsreg" )
endif( )
