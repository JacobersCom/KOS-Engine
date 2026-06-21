#pragma once


#include <fstream>
#include <print>

namespace Kos
{
	/*
	* The standard logging class for KOS
	* 
	* By default logs will be send to the output window and if logToFile is true, logs will also in KLog.txt
	*/
	class KLog
	{
	public:

		KLog() = default;

		void Initialize();
		bool SetLogToFile(const bool log);
		void WriteCmd();
	private:

		std::fstream logFile;
		bool logToFile;

	};
}