cmake_minimum_required( VERSION "3.7.1" )

set( VERSION 1.0.0 )

project( "ZTech" VERSION ${VERSION} )

# CMake Switches
option(ENABLE_TESTS "Enable tests" OFF)     # unit test build
option(USING_CPP17 "Use C++17 features" ON) # c++17 features

# Set a variable with the list of libraries
set(CMAKE_CXX_STANDARD 17)                  # We always use c++17, but not always use the features
set(ZTECH_ECS_LIBRARIES pthread)

# Set standard and add definition
if( USING_CPP17 )
    set(ZTECH_ECS_LIBRARIES pthread tbb)    # We need tbb for c++17 parallel for
    add_definitions( -DUSING_CPP17=1 )      # Set the definition to use in c++ code
else()
    add_definitions( -DUSING_CPP17=0 )      # Set the definition to use in c++ code
endif()

# Collect cpp files
file( GLOB_RECURSE SRC "${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp" )

# add public directory to include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)

# set static library with link to libraries
add_library( "ztech.ecs" STATIC "${SRC}" ) 
target_link_libraries( "ztech.ecs" "${ZTECH_ECS_LIBRARIES}" )

# Unit Test
if( ENABLE_TESTS )
    enable_testing( )
    MESSAGE( "Adding UnitTests" )
    add_executable( "ecstest" "unit_test/main.cpp" "${SRC}" )
    target_link_libraries( "ecstest" "${ZTECH_ECS_LIBRARIES}" )
    add_test( "UnitTest" "ecstest" )

    # registring the components test
    add_executable( "ecsreg" "unit_test/check_component_registration.cpp" "${SRC}" )
    target_link_libraries( "ecsreg" "${ZTECH_ECS_LIBRARIES}" )
    add_test( "EcsReg" "ecsreg" )

    # parallel for and entity creation test
    add_executable( "ecsallocone" "unit_test/check_entity_one_alloc.cpp" "${SRC}" )
    target_link_libraries( "ecsallocone" "${ZTECH_ECS_LIBRARIES}" )
    add_test( "EcsAllocOne" "ecsallocone" )

    # system registration test
    add_executable( "ecssysreg" "unit_test/check_system_def.cpp" "${SRC}" )
    target_link_libraries( "ecssysreg" "${ZTECH_ECS_LIBRARIES}" )
    add_test( "EcsSysReg" "ecssysreg" )

    # composer test
    add_executable( "ecscomposer" "unit_test/check_composer.cpp" "${SRC}" )
    target_link_libraries( "ecscomposer" "${ZTECH_ECS_LIBRARIES}" )
    add_test( "EcsComposer" "ecscomposer" )
endif( )
