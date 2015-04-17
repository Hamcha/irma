#include "LuaScript.h"

template<typename T> static T getResult(lua_State* state, int index);

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

template<typename R, typename ...T>
R LuaScript::CallFunction(const std::string name, const T... args) {
	lua_getglobal(state, name.c_str());
	int argCount = sizeof...(args);
	applyAllArgs(state, args...);
	lua_pcall(state, argCount, 1, 0);

	R result = getResult<R>(state, -1);
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

template<typename R, typename ...Args>
static std::function<R()> getAllArgs(lua_State* state, std::function<R(Args...)> function, const int item, const int limit) {
	auto bfunc = std::bind(function, getResult(state, item));
	if (limit == item) {
		return bfunc;
	}
	return getAllArgs(state, bfunc, item+1, limit);
}

template<> int getResult(lua_State* state, int index) { return lua_tointeger(state, index); }
template<> float getResult(lua_State* state, int index) { return (float)lua_tonumber(state, index); }
template<> double getResult(lua_State* state, int index) { return lua_tonumber(state, index); }
template<> const char* getResult(lua_State* state, int index) { return lua_tostring(state, index); }
template<> std::string getResult(lua_State* state, int index) { return std::string(lua_tostring(state, index)); }
template<> bool getResult(lua_State* state, int index) { return lua_toboolean(state, index) != 0; }

template<typename R, typename ...Args>
static int ctolua(std::function<R(Args...)> function, lua_State* state) {
	auto func = getAllArgs(state, function, 1, lua_gettop(state));
	R result = func();
	applyArg(state, result);
	return 1;
}

template<typename R, typename ...Args>
void LuaScript::BindFunction(const std::string fname, const std::function<R(Args...)> function) {
	//auto func = function;//std::bind(ctolua, function);
	//lua_pushcfunction(state, func);
	//lua_setglobal(state, fname.c_str());
}