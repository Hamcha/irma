#include "LuaScript.h"

LuaScript::LuaScript() {
	state = lua_open();
}

LuaScript::~LuaScript() {
	lua_close(state);
}