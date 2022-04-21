#pragma once

#include "ecs_types.h"
#include <vector>

namespace ztech::ecs
{
    class icomponent_array_interface
    {
    public:
        virtual void alloc( size_t count = 1 ) = 0;
        virtual void reset( entity_id_t id ) = 0;
        virtual void* get( entity_id_t id ) = 0;
        virtual size_t size( ) const = 0;

        virtual ~icomponent_array_interface( ) = default;
    };

    template< typename T >
    class component_array : public icomponent_array_interface
    {
    private:
        std::vector< T > datas;

    public:
        component_array( size_t reserve_count = 100 )
        {
            datas.reserve( reserve_count );
        }

        ~component_array( ) = default;

        // interface implementation
        virtual void alloc( size_t count = 1 ) override
        {
            std::vector< T > in_data( count );
            datas.insert( std::end( datas ), std::begin( in_data ), std::end( in_data ) );
        };
        virtual void* get( entity_id_t id ) override { return &datas[ id ]; }
        virtual size_t size( ) const override { return datas.size( ); }
        virtual void reset( entity_id_t id ) override { datas[ id ] = T{ }; }

        // for iteration
        inline const T* begin( ) const { return datas.cbegin( ); }
        inline const T* end( ) const { return datas.cend( ); }
        inline T* begin( ) { return datas.begin( ); }
        inline T* end( ) { return datas.end( ); }
        inline T& at( entity_id_t id ) { return datas[ id ]; }

        inline T& operator[]( entity_id_t id ) { return datas[ id ]; }

        inline void set( entity_id_t id, T&& in_data ) { datas[ id ] = std::move( in_data ); }
        
        inline void set( entity_id_t id, const T& in_data ) { datas[ id ] = in_data; }
        //inline decltype( datas )& get_array( ) { return datas; }

        /**
         * Replacing the component datas
         */
        //inline void replace( std::vector< T >&& in_datas ) { datas = std::move( in_datas ); }
        //inline void insert( const T& in_data ) { datas.push_back( in_data ); }
        //inline void insert( T&& in_data ) { datas.push_back( std::move( in_data ) ); }

        /**
         * Get Type Name
         */
        inline const char* get_type_name( ) const { return typeid( T ).name( ); }
    };
};