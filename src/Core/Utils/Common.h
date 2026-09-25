#pragma once
//NOTE: Should not be included difectly to a vulkan file! Use VkCommon for vulkan files
#define GATEWARE_ENABLE_CORE
#define GATEWARE_ENABLE_SYSTEM

#include <Gateware/Gateware.h>
#include <entt/entt.hpp>

#include <stdio.h>
#include <cstdint>

#define internal	  static
#define global		  static
#define local_persist static

//Base types
typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef float F32;
typedef double F64;
