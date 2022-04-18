#include "ecs/entity_array.h"
#include "ecs/system.h"
#include "definitions.h"
#include <atomic>
#include <chrono>
#include <set>

ztech::ecs::entity_array global_entities;

void init_system( ztech::ecs::entity_array* arr, ztech::ecs::entity_id_t id )
{
    
}

void move_system( ztech::ecs::entity_array* arr, ztech::ecs::entity_id_t id )
{
    auto loc = arr->get_component_data< car_location_t >( id );
    auto mov = arr->get_component_data< car_movement_t >( id );
    loc->x += mov->vx;
    loc->y += mov->vy;
}


int main( int argc, char* argv[] )
{
    // Create System
    printf( "Creating System\n" );
    ztech::ecs::system move_sys;
    move_sys.require< car_location_t >( );
    move_sys.require< car_movement_t >( );
    move_sys.require< car_lifetime_t >( );
    move_sys.register_function( move_system );

    // Checking the requirement on entities
    if ( move_sys.test( &global_entities ) ) return 1;

    // Register once the components
    printf( "Adding Location Component\n" );
    global_entities.register_component< car_location_t >( );
    if ( move_sys.test( &global_entities ) ) return 2;

    printf( "Adding Movement Component\n" );
    global_entities.register_component< car_movement_t >( );
    if ( move_sys.test( &global_entities ) ) return 2;

    printf( "Adding Lifetime Component\n" );
    global_entities.register_component< car_lifetime_t >( );
    if ( ! move_sys.test( &global_entities ) ) return 2;


    move_sys.execute( &global_entities );

    printf( "Success\n" );
    return 0;
}