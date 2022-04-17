# Ztech Engine ECS Module

## ECS

- Entity
- Component
- System

## Create Entities

- Create Entity Array with entity_array
- Register your components
- One component can registered only once

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

- Use free for release entities

Example:
```c++
ztech::ecs::entity_array entities;

auto id = entities.alloc( ); // allocate only one
entities.free( id );

// allocate 100 entitities
std::vector< ztech::ecs::entity_id_t > ids;
entities.alloc( ids, 100 );
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
});
```

