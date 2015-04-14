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
		throw LuaException(lua_tostring(state, -1));
	}
}

template<typename R, typename ...T>
R LuaScript::callFunction(const std::string name, const T... args) {
	lua_getglobal(state, name.c_str());
	applyAllArgs(state, args...);

}

template<typename I, typename ...T>
static void applyAllArgs(lua_State* state, const I item, const T... tail) {
	applyArg(state, item);
	if (sizeof...(tail)) {
		applyAllArgs(state, tail...);
	}
}

static void applyArg(lua_State* state, const int item) { lua_pushnumber(state, item); }
static void applyArg(lua_State *state, const float item) { lua_pushnumber(state, item); }
static void applyArg(lua_State *state, const double item) { lua_pushnumber(state, item); }
static void applyArg(lua_State *state, const std::string item) { lua_pushstring(state, item.c_str()); }
static void applyArg(lua_State *state, const char* item) { lua_pushstring(state, item); }
static void applyArg(lua_State *state, const bool item) { lua_pushboolean(state, item ? 1 : 0); }
