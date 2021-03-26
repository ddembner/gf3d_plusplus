#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>
#include <type_traits>

#define DEBUG_LOG(object) Debug::Log(__FILE__, __LINE__, __FUNCTION__, object)

class Debug
{

private:
	Debug();

public:
	static void Log(const char* f, int l, const char* func, const char* message);
	static void Log(const char* f, int l, const char* func, const std::string message);
	static void Log(const char* f, int l, const char* func, const int& message);
	static void Log(const char* f, int l, const char* func, const char message);
	static void Log(const char* f, int l, const char* func, const float message);
	static void Log(const char* f, int l, const char* func, const double message);
	static void Log(const char* f, int l, const char* func, const bool message);

	template <typename T> static void Log(const char* f, int l, const char* func, const T& obj) {
		
		std::stringstream ss;
		std::string file = f;
		
		file = file.substr(file.find_last_of("/\\") + 1);
		
		ss << file << ':' << func << ':' << l << "   " << typeid(obj).name() << ": " << obj;

		std::cout << ss.str() << std::endl;
	}

private:
	static char* variable_name(const char* name);
	static void console_log(const std::stringstream& ss);
	static void file_log(const std::stringstream& ss);
};

#endif // !__DEBUG_H__

