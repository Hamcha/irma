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

void LuaScript::ExecuteFile(const std::string file) {
	if (luaL_dofile(state, file.c_str())) {
		throw LuaException(file, lua_tostring(state, -1));
	}
}

template<> void applyArg(lua_State* state, const int item) { lua_pushnumber(state, item); }
template<> void applyArg(lua_State* state, const float item) { lua_pushnumber(state, item); }
template<> void applyArg(lua_State* state, const double item) { lua_pushnumber(state, item); }
template<> void applyArg(lua_State* state, const std::string item) { lua_pushstring(state, item.c_str()); }
template<> void applyArg(lua_State* state, const char* item) { lua_pushstring(state, item); }
template<> void applyArg(lua_State* state, const bool item) { lua_pushboolean(state, item ? 1 : 0); }

template<> int getResult(lua_State* state, int index) { return lua_tointeger(state, index); }
template<> float getResult(lua_State* state, int index) { return (float)lua_tonumber(state, index); }
template<> double getResult(lua_State* state, int index) { return lua_tonumber(state, index); }
template<> const char* getResult(lua_State* state, int index) { return lua_tostring(state, index); }
template<> std::string getResult(lua_State* state, int index) { return std::string(lua_tostring(state, index)); }
template<> bool getResult(lua_State* state, int index) { return lua_toboolean(state, index) != 0; }