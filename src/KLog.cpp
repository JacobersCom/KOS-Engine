#include "KLog.h"
#include "Common.h"


#include "iostream"
#include <fstream>
#include <filesystem>



global
std::string log_file_name = "KLog.txt";

global
bool log_to_file = false;

internal
void WriteToFile(const LogType type, std::string msg)
{
	const std::string pre_fix = (type == LogType::Info) ? "Info:" : (type == LogType::Warning) ? "Warning:" : "Error:";
	msg = pre_fix + " " + msg;

	//delete the previous log file
	bool first_log = true;
	if (first_log)
	{
		std::filesystem::remove(log_file_name);
		first_log = false;
	}

	std::ofstream fout;
	fout.open(log_file_name, std::ofstream::out | std::ofstream::app);

	if (fout.is_open())
	{
		fout << msg << std::endl;
		fout.close();
	}

}

internal
void WriteToOutput(const LogType type, std::string msg, ...)
{
	const std::string pre_fix = (type == LogType::Info) ? "[INFO]:" : (type == LogType::Warning) ? "[WARNING]:" : (type == LogType::Error) ? "[ERROR]:" : "[VULKAN ERROR]:";
	msg = pre_fix + " " + msg + "\n";
	std::cout << msg;
}


void KLog::WriteLog(const LogType type, std::string msg, ...)
{
	if (log_to_file)
	{
		WriteToFile(type, msg);
	}
	WriteToOutput(type, msg);
}

//NOTE: Add vulkan error file logging
void KLog::VulkanCheck(VkResult x)
{
	if (x != VK_SUCCESS)
	{
		WriteToOutput(LogType::VError, string_VkResult(x));
		abort();
	}
}

void KLog::SetLogToFile(const bool log)
{
	log_to_file = log;
}
