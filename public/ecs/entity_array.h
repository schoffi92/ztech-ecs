#pragma once

#include "ecs_types.h"
#include "component_array.h"
#include <atomic>
#include <functional>
#include <memory>
#include <shared_mutex>
#include <unordered_map>
#include <thread>

namespace ztech::ecs
{
    struct entity_validation_t
    {
        bool valid = false;
        void* linked_data = nullptr;
    };

    class entity_array
    {
        private:
            typedef std::pair< const char*, icomponent_array_interface* > pair_t;
            std::unordered_map< const char*, std::shared_ptr< icomponent_array_interface > > component_arrays;
            size_t reserved_count;

            std::shared_mutex component_arrays_mutex;
            std::atomic< size_t > entity_count;
            std::vector< entity_id_t > free_ids;

        public:
            entity_array( size_t reserved = 100 );

            template< typename T >
            inline void register_component( )
            {
                // Lock Mutex
                std::unique_lock< std::shared_mutex > lock( component_arrays_mutex );

                // One component can only exist once
                const char* type_name = typeid( T ).name( );
                if ( component_arrays.find( type_name ) != component_arrays.end( ) )
                {
                    return;
                }

                // register component
                std::shared_ptr< icomponent_array_interface > c = std::make_shared< component_array< T > >( reserved_count );
                //std::shared_ptr< icomponent_array_interface > c( ( icomponent_array_interface* )new component_array< T >( reserved_count ) );
                auto p = std::make_pair( type_name, c );
                component_arrays.insert( p );
            }

            template< typename T >
            inline std::shared_ptr< component_array< T > > get_component( )
            {
                std::shared_lock< std::shared_mutex > lock( component_arrays_mutex );
                const char* type_name = typeid( T ).name( );
                auto it = component_arrays.find( type_name );
                if ( it != component_arrays.end( ) ) return std::static_pointer_cast< component_array< T > >( it->second );
                return nullptr;
            }

            template< typename T >
            inline T* get_component_data( entity_id_t id )
            {
                const char* type_name = typeid( T ).name( );
                auto it = component_arrays.find( type_name );
                if ( it != component_arrays.end( ) ) return static_cast< T* >( it->second->get( id ) );
                return nullptr;
            }

            template< typename T >
            inline std::shared_ptr< icomponent_array_interface > get_component_interface( )
            {
                std::shared_lock< std::shared_mutex > lock( component_arrays_mutex );
                const char* type_name = typeid( T ).name( );
                auto it = component_arrays.find( type_name );
                if ( it != component_arrays.end( ) ) return it->second;
                return nullptr;
            }

            entity_id_t alloc( );
            void alloc( std::vector< entity_id_t >& out_entity_ids, size_t in_count = 1 );
            void free( entity_id_t id );

            template< typename T >
            inline void for_each( std::function< void( entity_id_t, T& ) > func )
            {
                std::shared_lock< std::shared_mutex > lock( component_arrays_mutex );
                auto valid_comp = get_component< entity_validation_t >( );
                auto comp = get_component< T >( );
                if ( comp != nullptr )
                {
                    for ( entity_id_t id = 0; id < comp->size( ); id++ )
                    {
                        if ( valid_comp->at( id ).valid ) func( id, comp->at( id ) );
                    }
                }
            }

            void for_each( std::function< void( entity_id_t ) > in_func );

            inline size_t size( ) const { return entity_count; }

            inline size_t get_component_count( ) const { return component_arrays.size( ); }

            inline size_t get_free_id_count( ) const { return free_ids.size( ); }

            /*
            template< std::size_t N >
            inline void for_each_parallel( std::function< void( entity_id_t ) > func )
            {
                std::shared_lock< std::shared_mutex > lock( component_arrays_mutex );
                const entity_id_t size = entity_count;
                const size_t part_size = size / N;
                entity_id_t start_id = 0;
                std::array< std::unique_ptr< std::thread >, N > threads;
                for ( int thread_index = 0; thread_index < N; thread_index++, start_id += part_size )
                {
                    entity_id_t end_id;
                    if ( thread_index + 1 == N ) end_id = size;
                    else end_id = start_id + part_size;
                    threads[ thread_index ] = std::make_unique< std::thread >( [&]( entity_id_t start, entity_id_t end )
                    {
                        for ( int index = start; index < end; index++ ) func( index );
                    }, start_id, end_id );
                }

                for ( int thread_index = 0; thread_index < N; thread_index++ )
                {
                    threads[ thread_index ]->join( );
                    threads[ thread_index ].reset( );
                }
            }
            */
    };
};
