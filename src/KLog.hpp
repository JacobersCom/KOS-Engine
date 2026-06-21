#pragma once

#include <string>

namespace Kos
{
	enum LogType
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
		bool SetLogToFile(const bool log);
		void WriteLog(LogType type, std::string msg);
	private:

		bool logToFile = false;


	};
}