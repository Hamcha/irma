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

template<> 
int getResult(lua_State* state, int index) {
	int value = lua_tointeger(state, index);
	lua_pop(state, 1);
	return value;
}

template<> 
float getResult(lua_State* state, int index) {
	float value = (float) lua_tonumber(state, index);
	lua_pop(state, 1);
	return value;
}

template<> 
double getResult(lua_State* state, int index) {
	double value = lua_tonumber(state, index);
	lua_pop(state, 1);
	return value;
}

template<> 
const char* getResult(lua_State* state, int index) {
	const char* value = lua_tostring(state, index);
	lua_pop(state, 1);
	return value;
}

template<> 
std::string getResult(lua_State* state, int index) {
	std::string value(lua_tostring(state, index));
	lua_pop(state, 1);
	return value;
}

template<> 
bool getResult(lua_State* state, int index) {
	bool value = lua_toboolean(state, index) != 0;
	lua_pop(state, 1);
	return value;
}

bool LuaScript::GlobalExists(const std::string varname) {
	lua_getglobal(state, varname.c_str());
	bool isnil = lua_isnil(state, -1);
	lua_pop(state, 1);
	return !isnil;
}