/**
 * @ Author Hunter Wilkinson
 * @ Create Time: 2026-04-15 08:23:00
 * 
 * Main_test.cpp is for all testing of all my systems
 * When you want to test a system just include the header needed 
 * and run the system you are testing!
 * 
 */

#include "KECS.h"

int main()
{
    printf("Testing!\n");
    int entity = kecs::NewEntity();
    printf("Entity Created: %i",entity);
}