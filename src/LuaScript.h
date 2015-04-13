#ifndef _LUASCRIPT_H
#define _LUASCRIPT_H

#include "lua.hpp"

class LuaScript {
private:
	lua_State* state;
public:
	LuaScript();
	~LuaScript();
};

#endif