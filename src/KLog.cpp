#include "KLog.hpp"
#include <fstream>
#include <print>
#include <filesystem>

namespace Kos
{
	namespace
	{
		std::string logFileName = "KLog.txt";
		bool logToFile = false;

		void WriteToFile(const LogType type, std::string msg)
		{
			const std::string preFix = (type == LogType::Info) ? "Info:" : (type == LogType::Warning) ? "Warning:" : "Error:";
			msg = preFix + " " + msg;

			//delete the previous log file
			bool firstLog = true;
			if (firstLog)
			{
				std::filesystem::remove(logFileName);
				firstLog = false;
			}

			std::ofstream fout;
			fout.open(logFileName, std::ofstream::out | std::ofstream::app);

			if (fout.is_open())
			{
				fout << msg << std::endl;
				fout.close();
			}

		}
		//Change this to use a typename for writing a log
		void WriteToOutput(const LogType type, std::string msg, ...)
		{


			const std::string preFix = (type == LogType::Info) ? "Info:" : (type == LogType::Warning) ? "Warning:" : "Error:";
			msg = preFix + " " + __FUNCTION__ + " " + msg + " " + "{}";


			//std::print(msg);
		}
	}

	void KLog::WriteLog(const LogType type, std::string msg, ...)
	{
		if (logToFile)
		{
			WriteToFile(type, msg);
		}
		WriteToOutput(type, msg);
	}

	void Kos::KLog::SetLogToFile(const bool log)
	{
		logToFile = log;
	}
}