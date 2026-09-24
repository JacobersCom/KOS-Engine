#pragma once
//NOTE: Should not be included difectly to a vulkan file! Use VkCommon for vulkan files

#include <stdio.h>
#include <cstdint>

#define _internal	  static
#define _global		  static
#define _local_persist static

//Base types
typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t	 S8;
typedef int16_t  S16;
typedef int32_t  S32;
typedef int64_t  S64;

typedef float	 F32;
typedef double	 F64;
