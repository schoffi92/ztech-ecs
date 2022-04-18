#pragma once

#include <chrono>

struct car_location_t
{
    double x, y = 0.0f;
};

struct car_movement_t
{
    double vx, vy = 0.0f;
};

struct car_lifetime_t
{
    bool exists = false;
    std::chrono::steady_clock::time_point born_at{};
};