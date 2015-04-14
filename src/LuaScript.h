#pragma once

#include <lua.hpp>
#include <string>
#include <stdexcept>

typedef class LuaScript LuaScript;

#include "Player.h"

class LuaScript {
private:
	lua_State* state;
public:
	LuaScript();
	~LuaScript();

	void ExecuteFile(const std::string file);
	void BindPlayer(Player* player);

	template<typename R, typename ...T>
	R callFunction(const std::string, const T...);
};

class LuaException : public std::runtime_error {
public:
	LuaException(std::string what) : std::runtime_error(what.c_str()) {}
};
