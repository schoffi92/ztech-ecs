#pragma once

#include <cstddef>

namespace ztech::ecs
{
    class icomponent_array_interface;
    class composer;
    class entity_array;
    class system;

    typedef std::size_t entity_id_t;

    struct entity_validation_t
    {
        bool valid = false;
        void* linked_data = nullptr;
    };
};