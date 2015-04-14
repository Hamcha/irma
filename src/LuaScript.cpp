#include "LuaScript.h"

template<typename T>
static T getResult(lua_State* state);

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
	int argCount = sizeof...(args);
	applyAllArgs(state, args...);
	lua_pcall(state, argCount, 1, 0);

	R result = getResult<R>(state);
	lua_pop(state, 1);
	return result;
}

template<typename I, typename ...T>
static void applyAllArgs(lua_State* state, const I item, const T... tail) {
	applyArg(state, item);
	if (sizeof...(tail)) {
		applyAllArgs(state, tail...);
	}
}

static void applyArg(lua_State* state, const int item) { lua_pushnumber(state, item); }
static void applyArg(lua_State* state, const float item) { lua_pushnumber(state, item); }
static void applyArg(lua_State* state, const double item) { lua_pushnumber(state, item); }
static void applyArg(lua_State* state, const std::string item) { lua_pushstring(state, item.c_str()); }
static void applyArg(lua_State* state, const char* item) { lua_pushstring(state, item); }
static void applyArg(lua_State* state, const bool item) { lua_pushboolean(state, item ? 1 : 0); }

template<>
int getResult(lua_State* state) {
    return lua_tointeger(state, -1);
}

template<>
float getResult(lua_State* state) {
    return lua_tonumber(state, -1);
}

template<>
double getResult(lua_State* state) {
    return lua_tonumber(state, -1);
}

template<>
const char* getResult(lua_State* state) {
    return lua_tostring(state, -1);
}

template<>
std::string getResult(lua_State* state) {
    return std::string(lua_tostring(state, -1));
}

template<>
bool getResult(lua_State* state) {
    return lua_toboolean(state, -1) != 0;
}
