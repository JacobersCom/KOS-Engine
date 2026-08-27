#pragma once

#include <string>

namespace Kos
{
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

		KLog() {};

		//void Initialize();
		static void SetLogToFile(const bool log);
		static void WriteLog(LogType type, std::string msg, ...);
	};
}
