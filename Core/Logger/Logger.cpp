#include "Logger.h"

#include <fstream>
#include <time.h>

namespace Debug {

	HANDLE Logger::hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	std::ostringstream Logger::logs = std::ostringstream();

	Logger::Logger(const char* file, const char* function, long const& line) {
#ifndef _DEBUG
		return;
#endif
		Tag(file, CYAN);
		Tag(function, GREEN);
		Tag(line, BLUE);

		std::cout << ' ';
		logs << ' ';
	}

	Logger::Logger(const char* tag, WORD const& color, const char* file, const char* function, long const& line) {
#ifndef _DEBUG
		return;
#endif
		Tag(tag, color);
		Logger::Logger(file, function, line);
	}

	void Logger::Dump() {
#ifndef _DEBUG
		return;
#endif
		std::ofstream ofs;
		ofs.open("Files/log_dump.txt", std::ios::app);

		const time_t t = time(nullptr);
		tm local;
		localtime_s(&local, &t);

		char timestr[26];
		asctime_s(timestr, sizeof timestr, &local);

		const std::string content = timestr + logs.str();
		ofs << content << '\n';

		ofs.close();
		logs.clear();
	}

}