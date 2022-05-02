#include "ecs/composer.h"
#include <thread>

#if USING_CPP17
#include <execution>    // This definied in c++17, but still not supported
#endif

void ztech::ecs::composer::run( )
{
    auto now = std::chrono::steady_clock::now( );
    float deltaSeconds = get_delta_seconds( );
    std::shared_lock< std::shared_mutex > lock( composer_links_mutex );

    #if USING_CPP17
        std::for_each( std::execution::par
            , std::begin( composer_links )
            , std::end( composer_links )
            , [&]( const std::pair< entity_array*, composer_link_t >& _pair )
            {
                const composer_link_t& link = _pair.second;
                for ( auto sys : link.systems )
                {
                    sys->execute( link.entities, deltaSeconds );
                }
            }
        );
    #else
        std::vector< std::unique_ptr< std::thread > > threads( composer_links.size( ) );
        int thread_index = 0;
        
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
    #endif
    last_run = std::move( now );
}