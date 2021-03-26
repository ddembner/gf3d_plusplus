#include "Debug.h"
#include <memory>
#include <typeinfo>
#include "vector2.h"

void Debug::Log(const char* f, int l, const char* func, const char* message) {
	
	std::stringstream ss;
	std::string file = f;

	file = file.substr(file.find_last_of("/\\") + 1);

	ss << file << ':' << func << ':' << l << "   const char*: " << message;
	console_log(ss);
}

void Debug::Log(const char* f, int l, const char* func, const std::string message) {

	std::stringstream ss;
	std::string file = f;

	file = file.substr(file.find_last_of("/\\") + 1);
	ss << file << ':' << func << ':' << l << "   std::string: " << message;
	console_log(ss);
}

void Debug::Log(const char* f, int l, const char* func, const int& message) {

	std::stringstream ss;
	std::string file = f;

	file = file.substr(file.find_last_of("/\\") + 1);
	ss << file << ':' << func << ':' << l << "   int: " << message;
	console_log(ss);
}

void Debug::Log(const char* f, int l, const char* func, const char message) {

	std::stringstream ss;
	std::string file = f;

	file = file.substr(file.find_last_of("/\\") + 1);
	ss << file << ':' << func << ':' << l << "   char: " << message;
	console_log(ss);
}

void Debug::Log(const char* f, int l, const char* func, const float message) {

	std::stringstream ss;
	std::string file = f;

	file = file.substr(file.find_last_of("/\\") + 1);
	ss << file << ':' << func << ':' << l << "   float: " << message;
	console_log(ss);
}

void Debug::Log(const char* f, int l, const char* func, const double message) {

	std::stringstream ss;
	std::string file = f;

	file = file.substr(file.find_last_of("/\\") + 1);
	ss << file << ':' << func << ':' << l << "   double: " << message;
	console_log(ss);
}

void Debug::Log(const char* f, int l, const char* func, const bool message) {

	std::stringstream ss;
	std::string file = f;

	file = file.substr(file.find_last_of("/\\") + 1);
	ss << file << ':' << func << ':' << l << "   bool: ";

	if (message == true)
		ss << "true";
	else
		ss << "false";

	console_log(ss);
}

char* Debug::variable_name(const char* variable) {
	return NULL;
}

void Debug::console_log(const std::stringstream& ss) {
	std::cout << ss.str() << std::endl;
}

void Debug::file_log(const std::stringstream& ss) {
	std::cout << ss.str() << std::endl;
}