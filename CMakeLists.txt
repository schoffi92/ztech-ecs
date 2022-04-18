cmake_minimum_required( VERSION "3.7.1" )

set( VERSION 1.0.0 )

project( "ZTech" VERSION ${VERSION} )
option(ENABLE_TESTS "Enable tests" OFF)

set(CMAKE_CXX_STANDARD 17)

file( GLOB_RECURSE SRC "${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp" )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)

add_library( "ztech.ecs" STATIC "${SRC}" )
#add_executable( "ecstest" "${SRC}" )

if( ENABLE_TESTS )
    enable_testing( )
    MESSAGE( "Adding UnitTests" )
    add_executable( "ecstest" "unit_test/main.cpp" "${SRC}" )
    add_test( "UnitTest" "ecstest" )

    add_executable( "ecsreg" "unit_test/check_component_registration.cpp" "${SRC}" )
    add_test( "EcsReg" "ecsreg" )

    add_executable( "ecsallocone" "unit_test/check_entity_one_alloc.cpp" "${SRC}" )
    add_test( "EcsAllocOne" "ecsallocone" )

    add_executable( "ecssysreg" "unit_test/check_system_def.cpp" "${SRC}" )
    add_test( "EcsSysReg" "ecssysreg" )

    add_executable( "ecscomposer" "unit_test/check_composer.cpp" "${SRC}" )
    add_test( "EcsComposer" "ecscomposer" )
endif( )
