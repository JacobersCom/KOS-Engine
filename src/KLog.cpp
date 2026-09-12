#include "KLog.h"


#include "iostream"
#include <fstream>
#include <filesystem>



static
std::string log_file_name = "KLog.txt";

static
bool log_to_file = false;

static
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

static
void WriteToOutput(const LogType type, std::string msg, ...)
{
	const std::string pre_fix = (type == LogType::Info) ? "[INFO]:" : (type == LogType::Warning) ? "[WARNING]:" : "[ERROR]:";
	msg = pre_fix + " " + msg;
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

void KLog::SetLogToFile(const bool log)
{
	log_to_file = log;
}
