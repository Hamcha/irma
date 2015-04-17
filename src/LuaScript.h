#pragma once

#include <lua.hpp>
#include <string>
#include <stdexcept>
#include <functional>

class LuaScript {
private:
	lua_State* state;
public:
	LuaScript();
	~LuaScript();

	void ExecuteFile(const std::string file);

	template<typename R, typename ...T>
	R CallFunction(const std::string, const T...);

	template<typename R, typename ...Args>
	void BindFunction(const std::string, const std::function<R(Args...)>);
};

class LuaException : public std::runtime_error {
public:
	std::string file;
	LuaException(std::string filename, std::string what) : std::runtime_error(what.c_str()) { file = filename; }
};
