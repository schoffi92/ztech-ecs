#include "ecs/composer.h"
#include <thread>

void ztech::ecs::composer::run( )
{
    std::shared_lock< std::shared_mutex > lock( composer_links_mutex );
    std::vector< std::unique_ptr< std::thread > > threads( composer_links.size( ) );
    int thread_index = 0;
    float deltaSeconds = get_delta_seconds( );
    auto now = std::chrono::steady_clock::now( );
    for ( auto it = std::begin( composer_links ); it != std::end( composer_links ); it++ )
    {
        threads[ thread_index ] = std::make_unique< std::thread >( [&]( const composer_link_t& link )
        {
            for ( auto sys : link.systems )
            {
                sys->execute( link.entities, deltaSeconds );
            }
        }, it->second);
        thread_index++;
    }

    for ( auto& th : threads )
    {
        th->join( );
        th.reset( );
    }
    last_run = std::move( now );
}