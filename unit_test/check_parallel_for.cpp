#include "ecs/entity_array.h"
#include "definitions.h"
#include <atomic>
#include <chrono>
#include <set>
#include <cmath>

ztech::ecs::entity_array global_entities;

template< std::size_t N >
int test_with_entity_count( )
{
    constexpr size_t new_entity_count = N;
    global_entities.clear( );

    std::array< double, new_entity_count > results;
    std::vector< ztech::ecs::entity_id_t > ids;
    global_entities.alloc( ids, new_entity_count );
    std::atomic< size_t > count = 0;
    auto onecore_start = std::chrono::steady_clock::now( );
    global_entities.for_each( [&count, &results]( ztech::ecs::entity_id_t _id )
    {
        ++count;
        auto i = sqrt( double( _id ) );
        results[ _id ] = pow( i, _id );
    });
    auto onecore_end = std::chrono::steady_clock::now( );
    auto onecore_dur = std::chrono::duration_cast< std::chrono::microseconds >( onecore_end - onecore_start );
    printf( "OneThread: Time: %zu, Count: %zu\n", onecore_dur.count( ), count.load( ) );
    if ( count != new_entity_count ) return 1;

    auto twocore_start = std::chrono::steady_clock::now( );
    count = 0;
    global_entities.for_each_parallel( [&count, &results]( ztech::ecs::entity_id_t _id )
    {
        ++count;
        auto i = sqrt( double(_id) );
        results[ _id ] = pow( i, _id );
    });
    auto twocore_end = std::chrono::steady_clock::now( );
    auto twocore_dur = std::chrono::duration_cast< std::chrono::microseconds >( twocore_end - twocore_start );
    printf( "Parallel:  Time: %zu, Count: %zu\n", twocore_dur.count( ), count.load( ) );
    if ( count != new_entity_count ) return 1;
    if ( N > 1000 && twocore_dur > onecore_dur ) return 1;

    // A test
    auto allcore_start = std::chrono::steady_clock::now( );
    count = 0;
    global_entities.for_each_parallel_old< 32 >( [&count, &results]( ztech::ecs::entity_id_t _id, std::size_t thread_index )
    {
        ++count;
        auto i = sqrt( double(_id) );
        results[ _id ] = pow( i, _id );
    });
    auto allcore_end = std::chrono::steady_clock::now( );
    auto allcore_dur = std::chrono::duration_cast< std::chrono::microseconds >( allcore_end - allcore_start );
    printf( "OldThread: Time: %zu, Count: %zu ( for_each_parallel_old with 32 thread )\n", allcore_dur.count( ), count.load( ) );
    if ( count != new_entity_count ) return 1;

    return 0;
}

int main( int argc, char* argv[] )
{
    // Register once the components
    printf( "Adding Components\n" );
    global_entities.register_component< car_location_t >( );
    global_entities.register_component< car_movement_t >( );

    if ( test_with_entity_count< 100 >( ) != 0 ) return 1;
    if ( test_with_entity_count< 1000 >( ) != 0 ) return 2;
    if ( test_with_entity_count< 10000 >( ) != 0 ) return 3;
    if ( test_with_entity_count< 100000 >( ) != 0 ) return 3;
    if ( test_with_entity_count< 1000000 >( ) != 0 ) return 4;
    //if ( test_with_entity_count< 2000000 >( ) != 0 ) return 5; @TODO: Above 1M it dies with segment fault

    printf( "Success\n" );
    return 0;
}