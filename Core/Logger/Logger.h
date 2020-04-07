#ifndef LOGGER_H
#define LOGGER_H

#include "../MACROS.h"

#include <Windows.h>
#include <sstream>
#include <iostream>

#define BLACK			0
#define DARK_BLUE		1
#define DARK_GREEN		2
#define DARK_CYAN		3
#define DARK_RED		4
#define DARK_PINK		5
#define DARK_YELLOW		6
#define DARK_BEIGE		7
#define GREY			8
#define BLUE			9
#define GREEN			10
#define CYAN			11
#define RED				12
#define PURPLE			13
#define YELLOW			14
#define WHITE			15

namespace Debug {

	class Logger {

		static HANDLE hConsole;

		static std::ostringstream logs;

	public:

		Logger(const char* file, const char* function, long const& line);
		Logger(const char* tag, WORD const& color, const char* file, const char* function, long const& line);

		template<typename Data>
		friend Logger operator<<(Logger logger, Data const& data);

		static void Dump();

	private:

		template<typename T>
		void Tag(T const& title, WORD const& color);

	};

	template<typename Data>
	Logger operator<<(Logger logger, Data const& data) {
#ifndef _DEBUG
		return logger;
#endif
		std::cout << data;
		Logger::logs << data;
		return logger;
	}

	template<typename T>
	void Logger::Tag(T const& title, WORD const& color) {
		SetConsoleTextAttribute(hConsole, WHITE);
		std::cout << '[';
		Logger::logs << '[';
		SetConsoleTextAttribute(hConsole, color);
		std::cout << title;
		Logger::logs << title;
		SetConsoleTextAttribute(hConsole, WHITE);
		std::cout << ']';
		Logger::logs << ']';
	}
}

#define Log Logger(__FILENAME__, __func__, __LINE__)
#define Warn Logger("WARNING", YELLOW, __FILENAME__, __func__, __LINE__)
#define Error Logger("ERROR", RED, __FILENAME__, __func__, __LINE__)
#define Custom(title, color) Logger(title, color, __FILENAME__, __func__, __LINE__)

#endif