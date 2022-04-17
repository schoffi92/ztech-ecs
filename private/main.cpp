#include "ecs/entity_array.h"
#include <cstdlib>

ztech::ecs::entity_array global_entities;

struct car_location_t
{
    double x, y = 0.0f;
};

struct car_movement_t
{
    double vx, vy = 0.0f;
};

void do_tick( )
{
    global_entities.for_each( []( ztech::ecs::entity_id_t id )
    {
        car_location_t* loc = global_entities.get_component_data< car_location_t >( id );
        car_movement_t* mov = global_entities.get_component_data< car_movement_t >( id );

        loc->x += mov->vx;
        loc->y += mov->vy;
    });
}

void print( )
{
    global_entities.for_each( []( ztech::ecs::entity_id_t id )
    {
        car_location_t* loc = global_entities.get_component_data< car_location_t >( id );
        car_movement_t* mov = global_entities.get_component_data< car_movement_t >( id );

        printf( "id: %zu { loc: %f,%f } { mov: %f,%f }\n", id, loc->x, loc->y, mov->vx, mov->vy );
    });
}

void free( std::vector< ztech::ecs::entity_id_t > ids )
{
    for ( auto it = std::begin( ids ); it != std::end( ids ); it++ )
    {
        global_entities.free( *it );
    }
}

int main( int argc, char* argv[] )
{
    std::vector< ztech::ecs::entity_id_t > ids;
    printf( "Initilaize Random\n" );
    std::srand( std::time( nullptr ) );

    printf( "Adding Components\n" );
    global_entities.register_component< car_location_t >( );
    global_entities.register_component< car_movement_t >( );

    auto id = global_entities.alloc( );
    global_entities.free( id );
    global_entities.free( id ); // invalid

    printf( "Add ten 100 entities\n" );
    global_entities.alloc( ids, 100 );
    free( ids );
    printf( "Entities count: %zu\n", global_entities.size( ) );

    global_entities.for_each( []( ztech::ecs::entity_id_t id )
    {
        auto loc_comp = global_entities.get_component< car_location_t >( );
        auto mov_comp = global_entities.get_component< car_movement_t >( );

        car_location_t* loc = (car_location_t*)loc_comp->get( id );
        car_movement_t* mov = (car_movement_t*)mov_comp->get( id );

        loc->x  = float( std::rand( ) % 100 ) / 100.0f;
        loc->y  = float( std::rand( ) % 100 ) / 100.0f;
        mov->vx = float( std::rand( ) % 100 ) / 100.0f;
        mov->vy = float( std::rand( ) % 100 ) / 100.0f;
    });

    //print( );
    do_tick( );
    //print( );
    /*
    printf( "Entity Count: %zu\n", global_entities.size( ) );
    free( );
    printf( "Entity Count: %zu\n", global_entities.size( ) );
    global_entities.alloc( ids, 50 );
    printf( "Entity Count: %zu\n", global_entities.size( ) );
    global_entities.alloc( ids, 100 );
    printf( "Entity Count: %zu\n", global_entities.size( ) );
    */

    return 0;
}