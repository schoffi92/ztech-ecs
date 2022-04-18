#include "ecs/entity_array.h"
#include "definitions.h"

ztech::ecs::entity_array global_entities;

int main( int argc, char* argv[] )
{
    // Register once the components
    printf( "Adding Components\n" );
    global_entities.register_component< car_location_t >( );
    global_entities.register_component< car_movement_t >( );

    // No Entity
    printf( "Check Entities\n" );
    if ( global_entities.size( ) != 0 ) return 1;
    
    // Allocated an entity
    printf( "Allocate entity\n" );
    auto id = global_entities.alloc( );
    if ( global_entities.size( ) != 1 ) return 2;

    // Free an entity
    printf( "Free entity %zu\n", id );
    global_entities.free( id );
    if ( global_entities.size( ) != 0 ) return 3;

    // We should Realloc the released entity
    printf( "Reallocate entity\n" );
    auto new_id = global_entities.alloc( );
    if ( id != new_id ) return 4;

    printf( "Success\n" );
    return 0;
}