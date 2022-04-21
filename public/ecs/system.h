#pragma once

#include "ecs_types.h"
#include "entity_array.h"
#include <memory>
#include <functional>
#include <set>

namespace ztech::ecs
{
    class system
    {
        private:
            typedef std::function< void( std::shared_ptr< entity_array >, entity_id_t, float ) > func_t;
            std::set< const char* > required_components;
            std::vector< func_t > functions;

        public:
            system( )
            {
                functions.reserve( 5 );
            }

            /**
             * Require Component
             */
            template< typename T >
            inline void require( )
            {
                required_components.insert( typeid( T ).name( ) );
            }

            inline void register_function( func_t in_func )
            {
                functions.push_back( in_func );
            }

            inline bool test( std::shared_ptr< entity_array > arr )
            {
                for ( auto type_name : required_components )
                {
                    if ( arr->get_component( type_name ) == nullptr ) return false;
                }
                return true;
            }

            inline void execute( std::shared_ptr< entity_array > arr, float deltaSeconds = 0.0f, size_t start = 0, size_t end = 0 )
            {
                if ( ! test( arr ) ) return;

                for ( auto it = std::begin( functions ); it != std::end( functions ); it++ )
                {
                    arr->for_each( std::bind( *it, arr, std::placeholders::_1, deltaSeconds ), start, end );
                }
            }

            template< std::size_t N >
            inline void execute_parallel( std::shared_ptr< entity_array > arr, float deltaSeconds = 0.0f )
            {
                if ( ! test( arr ) ) return;
                for ( auto it = std::begin( functions ); it != std::end( functions ); it++ )
                {
                    arr->for_each_parallel< N >( std::bind( *it, arr, std::placeholders::_1, deltaSeconds ) );
                }
            }
    };
};