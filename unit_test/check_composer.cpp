#include "ecs/composer.h"
#include "ecs/entity_array.h"
#include "ecs/system.h"
#include "definitions.h"
#include <cstdlib>
#include <vector>
#include <numeric>

constexpr size_t entity_count = 100;

static ztech::ecs::composer global_composer;
static std::shared_ptr< ztech::ecs::entity_array > car_entities;
static std::shared_ptr< ztech::ecs::system > car_system;
static std::atomic< int > system_excution_count = 0;


void move_car( std::shared_ptr< ztech::ecs::entity_array > arr, ztech::ecs::entity_id_t id )
{
    auto loc = arr->get_component_data< car_location_t >( id );
    auto mov = arr->get_component_data< car_movement_t >( id );

    loc->x += mov->vx;
    loc->y += mov->vy;

    ++system_excution_count;
}

int init_car_entities( )
{
    printf( "Initalize Car Entities\n" );
    // Init Car Entities
    car_entities = std::make_shared< ztech::ecs::entity_array >( );
    car_entities->register_component< car_location_t >( );
    car_entities->register_component< car_movement_t >( );
    std::vector< ztech::ecs::entity_id_t > entity_ids;
    car_entities->alloc( entity_ids, entity_count );
    if ( entity_ids.size( ) != entity_count )
    {
        printf( "Allocated entity count ( %zu ) does not equal with requested ( %zu )\n", entity_ids.size( ), entity_count );
        return 1;
    }

    std::vector< ztech::ecs::entity_id_t > ids;
    car_entities->for_each( [&ids]( ztech::ecs::entity_id_t id )
    {
        auto loc = car_entities->get_component_data< car_location_t >( id );
        auto mov = car_entities->get_component_data< car_movement_t >( id );
        loc->x = float( std::rand( ) % 100 ) * 0.01f;
        loc->y = float( std::rand( ) % 100 ) * 0.01f;
        mov->vx = float( std::rand( ) % 100 ) * 0.01f;
        mov->vy = float( std::rand( ) % 100 ) * 0.01f;

        ids.push_back( id );
    });

    if ( ids.size( ) != entity_count ) return 1;
    return 0;
}

int init_car_system( )
{
    printf( "Initalize Car System\n" );
    car_system = std::make_shared< ztech::ecs::system >( );
    car_system->require< car_location_t >( );
    car_system->require< car_movement_t >( );
    car_system->register_function( move_car );

    return 0;
}

int main( int argc, char* argv[] )
{
    std::srand( std::time( nullptr ) );
    printf( "Delta Seconds: %f\n", global_composer.get_delta_seconds( ) );

    {
        auto result = init_car_entities( );
        if ( result != 0 ) return result;
    }

    {
        auto result = init_car_system( );
        if ( result != 0 ) return result;
    }
    
    printf( "Run Composer Tick with %zu cars\n", entity_count );
    global_composer.register_link( car_entities, car_system );
    global_composer.run( );
    if ( system_excution_count.load( ) != entity_count )  return 2;

    {
        system_excution_count = 0;
        std::vector< size_t > release_ids( entity_count / 2 );
        std::iota( std::begin( release_ids ), std::end( release_ids ), 0 );
        car_entities->free( release_ids );

        printf( "Run Composer Tick with %zu car\n", entity_count / 2 );
        global_composer.register_link( car_entities, car_system );
        global_composer.run( );
        if ( system_excution_count.load( ) != ( entity_count / 2 ) )
        {
            printf( "System Exectuion count is %zu, but it should be %zu\n", entity_count, ( entity_count / 2 ) );
            return 3;
        }
    }

    printf( "Success\n" );
    return 0;
}