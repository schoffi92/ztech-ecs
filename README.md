# Ztech Engine ECS Module

## ECS

- Entity
- Component
- System

## Create Entities

- Create Entity Array with entity_array
- Register your components
- One component can registered only once
- Your component registration should be the first thing you do
- After adding/releasing entities should not register any other components

Example:
```c++
struct car_location_t
{
    double x, y = 0.0f;
};

struct car_movement_t
{
    double vx, vy = 0.0f;
};

ztech::ecs::entity_array entities;

{
    entities.register_component< car_location_t >( );
    entities.register_component< car_movement_t >( );
}
```

## Add entities

- Locking the entities
- Use alloc for allocate entities

Example:
```c++
ztech::ecs::entity_array entities;

auto id = entities.alloc( ); // allocate only one

// allocate 100 entitities
std::vector< ztech::ecs::entity_id_t > ids;
entities.alloc( ids, 100 );
```

## Free entity

- Locking the entities
- Use free for release entities

Example:
```c++
ztech::ecs::entity_array entities;

auto id = entities.alloc( ); // allocate only one
entities.free( id );
```

## Iterating through entities

- Use for_each method what lock the components with shared lock

Example:
```c++
ztech::ecs::entity_array entities;

entities.for_each( []( ztech::ecs::entity_id_t id )
{
    // also the invalid entities will be filtered out
    // do something
    auto loc_comp = entities.get_component< car_location_t >( ); // asking for component array
    car_movement_t* mov_comp = entities.get_component_data< car_movement_t >( id ); // asking for the data from the component
    ( *loc_comp )[ id ].x = 10.0f;
    loc_comp->at( id ).y = 10.0f;

    mov_comp->vx = 10.0f;
    mov_comp->vy = 12.0f;
});

// Iterating through entities with two thread
entities.for_each_parallel< 2 >( []( ztech::ecs::entity_id_t id )
{
    // also the invalid entities will be filtered out
    // do something
});
```

## Remove multiple ids

- Use for_each method to collect removable elements
- call free with the vector
- free unique lock the components ( so we cannot call free in for_each )

Example:
```c++
std::vector< ztech::ecs::entity_id_t > removed_ids;
entities.for_each( [&removed_ids]( ztech::ecs::entity_id_t id )
{
    // also the invalid entities will be filtered out
    // do something
    if ( id % 2 == 1 ) removed_ids.push_back( id );
});

entities.free( removed_ids );
```
