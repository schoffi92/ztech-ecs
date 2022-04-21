#include "ecs/entity_array.h"
#include "ecs/system.h"
#include "definitions.h"
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <set>

std::shared_ptr< ztech::ecs::entity_array > global_entities;

void init_system( std::shared_ptr< ztech::ecs::entity_array > arr, ztech::ecs::entity_id_t id, float deltaSeconds )
{
    auto lt = arr->get_component_data< car_lifetime_t >( id );
    if ( ! lt->exists )
    {
        auto loc = arr->get_component_data< car_location_t >( id );
        auto mov = arr->get_component_data< car_movement_t >( id );

        loc->x  = float( std::rand( ) % 100 ) / 100.0f;
        loc->y  = float( std::rand( ) % 100 ) / 100.0f;
        mov->vx = float( std::rand( ) % 100 ) / 100.0f;
        mov->vy = float( std::rand( ) % 100 ) / 100.0f;
        lt->exists = true;
        lt->born_at = std::chrono::steady_clock::now( );
    }
}

void move_system( std::shared_ptr< ztech::ecs::entity_array > arr, ztech::ecs::entity_id_t id, float deltaSeconds )
{
    auto loc = arr->get_component_data< car_location_t >( id );
    auto mov = arr->get_component_data< car_movement_t >( id );
    loc->x += mov->vx;
    loc->y += mov->vy;
}


int main( int argc, char* argv[] )
{
    global_entities = std::make_shared< ztech::ecs::entity_array >( );
    std::srand( std::time( nullptr ) );

    // Create System
    printf( "Creating System\n" );
    ztech::ecs::system move_sys;
    move_sys.require< car_location_t >( );
    move_sys.require< car_movement_t >( );
    move_sys.require< car_lifetime_t >( );
    move_sys.register_function( init_system );
    move_sys.register_function( move_system );

    // Checking the requirement on entities
    if ( move_sys.test( global_entities ) ) return 1;

    // Register once the components
    printf( "Adding Location Component\n" );
    global_entities->register_component< car_location_t >( );
    if ( move_sys.test( global_entities ) ) return 2;

    printf( "Adding Movement Component\n" );
    global_entities->register_component< car_movement_t >( );
    if ( move_sys.test( global_entities ) ) return 2;

    printf( "Adding Lifetime Component\n" );
    global_entities->register_component< car_lifetime_t >( );
    if ( ! move_sys.test( global_entities ) ) return 2;


    move_sys.execute( global_entities );

    printf( "Success\n" );
    return 0;
}