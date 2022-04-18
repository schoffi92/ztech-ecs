#include "ecs/entity_array.h"

ztech::ecs::entity_array::entity_array( size_t reserved )
    : reserved_count( reserved )
{
    entity_count = 0;
    register_component< entity_validation_t >( );
    component_arrays.reserve( 5 );
    free_ids.reserve( 5 );
}

ztech::ecs::entity_id_t ztech::ecs::entity_array::alloc( )
{
    ++entity_count;
    auto valid_comp = get_component< entity_validation_t >( );
    std::unique_lock< std::shared_mutex > lock( component_arrays_mutex );
    if ( free_ids.size( ) > 0 )
    {
        auto id = free_ids.back( );
        free_ids.pop_back( );
        valid_comp->at( id ).valid = true;
        return id;
    }
    entity_id_t id = valid_comp->size( );
    for ( auto it = std::begin( component_arrays ); it != std::end( component_arrays ); it++ )
    {
        it->second->alloc( 1 );
        if ( it->first == typeid( entity_validation_t ).name( ) ) static_cast< entity_validation_t* >( it->second->get( id ) )->valid = true;
    }
    return id;
}

void ztech::ecs::entity_array::alloc( std::vector< ztech::ecs::entity_id_t >& out_entity_ids, size_t in_count )
{
    entity_count.fetch_add( in_count );
    auto count = in_count;
    auto valid_comp = get_component< entity_validation_t >( );
    std::unique_lock< std::shared_mutex > lock( component_arrays_mutex );
    if ( free_ids.size( ) > in_count )
    {
        // free_ids vector has more elements then the alloc entity count
        out_entity_ids.insert( std::begin( out_entity_ids ), std::end( free_ids ) - in_count, std::end( free_ids ) );
        free_ids.resize( free_ids.size( ) - in_count );
        for ( auto it = std::begin( out_entity_ids ); it != std::end( out_entity_ids ); it++ ) valid_comp->at( *it ).valid = true;
        return;
    }
    else if ( free_ids.size( ) > 0 )
    {
        // we have free_ids and contain less element than the alloc entity count
        out_entity_ids = std::move( free_ids );
        count -= out_entity_ids.size( );
        for ( auto it = std::begin( out_entity_ids ); it != std::end( out_entity_ids ); it++ ) valid_comp->at( *it ).valid = true;
    }
    // lets add some new entities
    entity_id_t start_id = valid_comp->size( );
    entity_id_t end_id = start_id + count;
    for ( auto it = std::begin( component_arrays ); it != std::end( component_arrays ); it++ ) it->second->alloc( count );
    for ( entity_id_t id = start_id; id < end_id; id++ ) valid_comp->at( id ).valid = true;
}


void ztech::ecs::entity_array::free( entity_id_t id )
{
    // Out Of Range
    auto valid_comp = get_component< entity_validation_t >( );
    if ( id >= valid_comp->size( ) ) return;

    // Check if the entity already released
    std::unique_lock< std::shared_mutex > lock( component_arrays_mutex );
    if ( ! valid_comp->at( id ).valid )
    {
        // the entity already released
        printf( "Trying to free invalid entity %zu\n", id );
        return;
    }

    // The entity is valid
    --entity_count;
    valid_comp->reset( id );
    for ( auto it = std::begin( component_arrays ); it != std::end( component_arrays ); it++ ) it->second->reset( id );
    free_ids.push_back( id );
}

void ztech::ecs::entity_array::for_each( std::function< void( entity_id_t ) > in_func )
{
    auto valid_comp = get_component< entity_validation_t >( );
    std::shared_lock< std::shared_mutex > lock( component_arrays_mutex );
    entity_id_t size = entity_count;
    for ( entity_id_t id = 0; id < size; id++ )
    {
        if ( ! valid_comp->at( id ).valid ) continue;
        in_func( id );
    }
}