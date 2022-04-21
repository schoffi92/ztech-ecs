#pragma once

#include "ecs_types.h"
#include "entity_array.h"
#include "system.h"
#include <chrono>
#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

namespace ztech::ecs
{
    struct composer_link_t
    {
        std::shared_ptr< entity_array > entities;
        std::vector< std::shared_ptr< system > > systems;

        composer_link_t( )
        {
            systems.reserve( 5 );
        }
    };

    class composer
    {
    private:
        std::unordered_map< entity_array*, composer_link_t > composer_links;
        std::shared_mutex composer_links_mutex;
        std::chrono::steady_clock::time_point last_run;

    public:
        composer( )
            : last_run( std::chrono::steady_clock::now( ) )
        {
            composer_links.reserve( 5 );
        }

        inline const decltype( composer_links )& get_links( ) const { return composer_links; }

        inline void register_link( std::shared_ptr< entity_array > arr, std::shared_ptr< system > sys )
        {
            std::unique_lock< std::shared_mutex > lock( composer_links_mutex );
            auto& link = composer_links[ arr.get( ) ];
            link.entities = arr;
            link.systems.push_back( sys );
        }

        inline float get_delta_seconds( ) const
        {
            auto now = std::chrono::steady_clock::now( );
            auto dur = float( std::chrono::duration_cast< std::chrono::milliseconds >( now - last_run ).count( ) ) * 0.001f;
            return dur;
        }

        void run( );
    };
}