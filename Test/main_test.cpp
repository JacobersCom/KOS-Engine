/**
 * @ Author Hunter Wilkinson
 * @ Create Time: 2026-04-15 08:23:00
 * 
 * Main_test.cpp is for all testing of all my systems
 * 
 * 1. Configure with tests ON (default)
 * cmake -B build -S . -DBUILD_TESTS=ON
 * 2. Now turn tests OFF
 * cmake -B build -S . -DBUILD_TESTS=OFF
 */


#include "Engine-Core/include/ecs/KECS.h"

int main()
{
    kecs::NewEntity();
}