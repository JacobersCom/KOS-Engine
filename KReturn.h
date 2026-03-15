/*
MReturns is the beginning of the logging system for the MAUS engine. HELD within the MW namespace
- Goal
	- Have return codes for different vulkan and windows API error that are ambiguous 
*/
#pragma once

#ifndef KRETURN_H
#define KRETURN_H



namespace KE
{
	enum class KReturn : int
	{
		K_PROGRAM_CLOSED = 3,

		K_WINDOW_CREATION_FAILED = -2,
		K_WINDOW_CREATION_SUCCESS = 2,

		K_LOGICAL_DEVICE_CREATION_SUCCESS = 4,
		K_LOGICAL_DEVICE_CREATION_FAILED = -4,

		K_SUCCESS = 0,
		K_FAILURE = 1,
	};

}


#endif // MRETURN_H