#pragma once

#include <vulkan/vk_enum_string_helper.h>

//Add loging for Vulkan errors

enum class LogType
{
	Info,
	Warning,
	Error,
	VError,
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
	static void VulkanCheck(VkResult x);
};

