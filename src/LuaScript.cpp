#include "LuaScript.h"

LuaScript::LuaScript() {
	state = luaL_newstate();
	luaL_openlibs(state);
}

LuaScript::~LuaScript() {
	if (state) {
		lua_close(state);
	}
}

void LuaScript::ExecuteFile(std::string file) {
	if (luaL_dofile(state, file.c_str())) {
		throw LuaException(lua_tostring(state, -1));
	}
}
