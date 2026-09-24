#pragma once

#define GATEWARE_ENABLE_CORE
#define GATEWARE_ENABLE_SYSTEM
#define GATEWARE_ENABLE_GRAPHICS

#define GATEWARE_DISABLE_GDIRECTX11SURFACE
#define GATEWARE_DISABLE_GDIRECTX12SURFACE
#define GATEWARE_DISABLE_GRASTERSURFACE
#define GATEWARE_DISABLE_GOPENGLSURFACE

#include <stdio.h>
#include <cstdint>
#include "../Core/Logger/KLog.h"
#include "../../third_party/Gateware.h"

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
