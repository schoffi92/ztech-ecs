#include "ecs/entity_array.h"
#include "definitions.h"

ztech::ecs::entity_array global_entities;

int main( int argc, char* argv[] )
{
    // Register once the components
    printf( "Register once the components\n" );
    global_entities.register_component< car_location_t >( );
    global_entities.register_component< car_movement_t >( );
    if ( global_entities.get_component_count( ) != 3 ) return 1;

    // Try to register once again the components
    printf( "Try to register once again the components\n" );
    global_entities.register_component< car_location_t >( );
    global_entities.register_component< car_movement_t >( );
    if ( global_entities.get_component_count( ) != 3 ) return 2;

    // Check the components pointers
    printf( "Check components pointers\n" );
    auto loc_comp = global_entities.get_component< car_location_t >( );
    auto mov_comp = global_entities.get_component< car_movement_t >( );
    if ( (void*)loc_comp.get( ) == (void*)mov_comp.get( ) ) return 3;
    if ( loc_comp.get( ) == nullptr ) return 3;
    if ( mov_comp.get( ) == nullptr ) return 3;

    // Check components data pointers
    printf( "Check components data pointers\n" );
    auto loc = global_entities.get_component_data< car_location_t >( 0 );
    auto mov = global_entities.get_component_data< car_movement_t >( 0 );
    if ( (void*)loc == (void*)mov ) return 4;
    if ( loc == nullptr ) return 4;
    if ( mov == nullptr ) return 4;

    return 0;
}