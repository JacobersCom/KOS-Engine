#pragma once

<<<<<<< HEAD:src/Core/Logger/KLog.h
//Add loging for Vulkan errors
=======
#include <string>

>>>>>>> parent of 028c38d (Added Vulkan logging, and created a helper class for NonCopyable objects):src/KLog.h

#include <iostream>
#include <string>

enum class LogType
{
	Info,
	Warning,
	Error
};


/*
* The standard logging class for KOS
* 
* By default logs will be send to the output window and if logToFile is true, logs will also in KLog.txt
*/
class KLog
{
public:

	static void SetLogToFile(const bool log);
	static void WriteLog(LogType type, std::string msg, ...);
};

