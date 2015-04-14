#ifndef _LUASCRIPT_H
#define _LUASCRIPT_H

#include <lua.hpp>
#include <string>
#include <stdexcept>

class LuaScript {
private:
	lua_State* state;
public:
	LuaScript();
	~LuaScript();

	void ExecuteFile(std::string file);

	template<typename R, typename ...T>
	R callFunction(std::string name, T...) {}
};

class LuaException : public std::runtime_error {
public:
	LuaException(std::string what) : std::runtime_error(what.c_str()) {}
};

#endif
