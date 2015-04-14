#pragma once

#include <lua.hpp>
#include <string>
#include <stdexcept>

class LuaScript {
private:
	lua_State* state;
public:
	LuaScript();
	~LuaScript();

	void ExecuteFile(const std::string file);

	template<typename R, typename ...T>
	R callFunction(const std::string, const T...);

	template<typename T>
	void bindFunction(const std::string, T* function);
};

class LuaException : public std::runtime_error {
public:
	std::string file;
	LuaException(std::string filename, std::string what) : std::runtime_error(what.c_str()) { file = filename; }
};
