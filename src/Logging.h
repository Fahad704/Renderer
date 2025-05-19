#pragma once
#include <iostream>
#include <Windows.h>
#define APPNAME "Renderer"
void setConsoleAttribute(WORD attribute) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, attribute);
}
std::ostream& red(std::ostream& os) {
	setConsoleAttribute(FOREGROUND_RED | FOREGROUND_INTENSITY);
	return os;
}
std::ostream& yellow(std::ostream& os) {
	setConsoleAttribute(FOREGROUND_RED | FOREGROUND_GREEN);
	return os;
}
std::ostream& white(std::ostream& os) {
	setConsoleAttribute(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return os;
}
std::ostream& green(std::ostream& os) {
	setConsoleAttribute(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	return os;
}
void printLive(const std::string& message) {
	static std::string lastLine;
	int pad = std::max<int>(0, lastLine.size() - message.size());
	std::cout << "\r" << message << std::string(pad, ' ')<< std::flush;
	lastLine = message;
}
#define CLEARLINE "\r" << std::string(100,' ') << "\r"
#define LOG_INFO(x) std::cout << CLEARLINE <<"["<<APPNAME<<"]:"<<x;
#define LOG_WARN(x) std::cout << CLEARLINE << yellow <<"["<<APPNAME<<"]:[WARN]:"<< x << white << "\n";
#define LOG_ERROR(x) std::cout << CLEARLINE << red <<"["<<APPNAME<<"]:[ERROR]:"<< x << white << "\n";
#define LOG_SUCCESS(x) std::cout << CLEARLINE << green <<"["<<APPNAME<<"]:"<< x << white << "\n";