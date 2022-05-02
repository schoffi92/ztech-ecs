cmake_minimum_required( VERSION "3.7.1" )

set( VERSION 1.0.0 )

project( "ZTech" VERSION ${VERSION} )
option(ENABLE_TESTS "Enable tests" OFF)
option(USING_CPP17 "Use C++17 features" ON)
set(ZTECH_ECS_LIBRARIES pthread)

if( USING_CPP17 )
    set(CMAKE_CXX_STANDARD 17)
    set(ZTECH_ECS_LIBRARIES pthread tbb)
    add_definitions( -DUSING_CPP17=1 )
else()
    set(CMAKE_CXX_STANDARD 17)
    add_definitions( -DUSING_CPP17=0 )
endif()

file( GLOB_RECURSE SRC "${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp" )

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)

add_library( "ztech.ecs" STATIC "${SRC}" ) 
target_link_libraries( "ztech.ecs" "${ZTECH_ECS_LIBRARIES}" )

#add_executable( "ecstest" "${SRC}" )
# -pthread -ltbb
if( ENABLE_TESTS )
    enable_testing( )
    MESSAGE( "Adding UnitTests" )
    add_executable( "ecstest" "unit_test/main.cpp" "${SRC}" )
    target_link_libraries( "ecstest" "${ZTECH_ECS_LIBRARIES}" )
    add_test( "UnitTest" "ecstest" )

    add_executable( "ecsreg" "unit_test/check_component_registration.cpp" "${SRC}" )
    target_link_libraries( "ecsreg" "${ZTECH_ECS_LIBRARIES}" )
    add_test( "EcsReg" "ecsreg" )

    add_executable( "ecsallocone" "unit_test/check_entity_one_alloc.cpp" "${SRC}" )
    target_link_libraries( "ecsallocone" "${ZTECH_ECS_LIBRARIES}" )
    add_test( "EcsAllocOne" "ecsallocone" )

    add_executable( "ecssysreg" "unit_test/check_system_def.cpp" "${SRC}" )
    target_link_libraries( "ecssysreg" "${ZTECH_ECS_LIBRARIES}" )
    add_test( "EcsSysReg" "ecssysreg" )

    add_executable( "ecscomposer" "unit_test/check_composer.cpp" "${SRC}" )
    target_link_libraries( "ecscomposer" "${ZTECH_ECS_LIBRARIES}" )
    add_test( "EcsComposer" "ecscomposer" )
endif( )
