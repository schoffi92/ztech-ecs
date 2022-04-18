#include "ecs/entity_array.h"
#include "definitions.h"
#include <set>

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
    if ( global_entities.get_free_id_count( ) != 1 ) return 3;

    // We should Realloc the released entity
    printf( "Reallocate entity\n" );
    auto new_id = global_entities.alloc( );
    if ( id != new_id ) return 4;
    if ( global_entities.get_free_id_count( ) != 0 ) return 4;

    // Alloc another one it should be different from the last
    printf( "Allocate a second entity\n" );
    id = global_entities.alloc( );
    if ( id == new_id ) return 5;

    {
        // Counting Unique ids
        printf( "Counting Unique ids should be two\n" );
        std::set< ztech::ecs::entity_id_t > ids;
        global_entities.for_each( [&ids]( ztech::ecs::entity_id_t _id )
        {
            printf( "Id: '%zu'\n", _id );
            ids.insert( _id );
        });
        if ( ids.size( ) != 2 ) return 6;
    }

    {
        // Counting Unique ids
        printf( "Release Entity ( %zu ) and keep ( %zu )\n", new_id, id );
        global_entities.free( new_id );
        if ( global_entities.get_free_id_count( ) != 1 ) return 7;

        printf( "Counting Unique ids\n" );
        std::set< ztech::ecs::entity_id_t > ids;
        global_entities.for_each( [&ids]( ztech::ecs::entity_id_t _id )
        {
            printf( "Id: '%zu'\n", _id );
            ids.insert( _id );
        });
        printf( "Counting Unique ids ( %zu )\n", ids.size( ) );
        if ( ids.size( ) != 1 ) return 7;
    }

    printf( "Success\n" );
    return 0;
}