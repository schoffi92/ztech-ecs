#include "ecs/entity_array.h"
#include "definitions.h"
#include <atomic>
#include <chrono>
#include <set>
#include <cmath>

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
        // Trying the parallel for
        printf( "Trying to for_each parallel\n" );
        std::atomic< size_t > count = 0;
        auto start = std::chrono::steady_clock::now( );
        global_entities.for_each_parallel( [&count, &start]( ztech::ecs::entity_id_t _id )
        {
            using namespace std::chrono_literals;
            ++count;
            std::this_thread::sleep_until( start + 100ms );
        });
        auto end = std::chrono::steady_clock::now( );
        auto dur = std::chrono::duration_cast< std::chrono::milliseconds >( end - start );
        printf( "Time: %zu, Count: %zu\n", dur.count( ), count.load( ) );
        if ( dur.count( ) >= 200 ) return 7;
        if ( count != 2 ) return 7;
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
        if ( ids.size( ) != 1 ) return 8;
    }

    {
        // Trying the parallel for
        printf( "Trying to for_each parallel\n" );
        std::atomic< size_t > count = 0;
        auto start = std::chrono::steady_clock::now( );
        global_entities.for_each_parallel( [&count, &start]( ztech::ecs::entity_id_t _id )
        {
            using namespace std::chrono_literals;
            ++count;
            std::this_thread::sleep_until( start + 100ms );
        });
        auto end = std::chrono::steady_clock::now( );
        auto dur = std::chrono::duration_cast< std::chrono::milliseconds >( end - start );
        printf( "Time: %zu, Count: %zu\n", dur.count( ), count.load( ) );
        if ( count != 1 ) return 7;
    }

    {
        constexpr size_t new_entity_count = 100000;
        std::array< double, new_entity_count + 1 > results;
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
        if ( count != new_entity_count + 1 ) return 8;

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
        printf( "TwoThread: Time: %zu, Count: %zu\n", twocore_dur.count( ), count.load( ) );
        if ( count != new_entity_count + 1 ) return 8;
        if ( twocore_dur > onecore_dur ) return 8;

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
        if ( count != new_entity_count + 1 ) return 8;
    }

    printf( "Success\n" );
    return 0;
}