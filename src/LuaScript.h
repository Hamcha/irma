#pragma once

#include <lua.hpp>
#include <string>
#include <stdexcept>
#include <functional>

template<typename R>
void applyArg(lua_State*, const R);

template<typename R>
R getResult(lua_State* state, int index = -1);

class LuaException : public std::runtime_error {
public:
	std::string file;
	LuaException(std::string filename, std::string what) : std::runtime_error(what.c_str()) { file = filename; }
};

class LuaScript {
private:
	lua_State* state;

	template<typename I, typename ...T>
	static void applyAllArgs(lua_State* state, const I item, const T... tail) {
		applyArg(state, item);
		if (sizeof...(tail)) {
			applyAllArgs(state, tail...);
		}
	}

	static std::function<void(void)> getAllArgs(lua_State* state, const std::function<void(void)>& function) {
		return function;
	}

	template<typename R>
	static std::function<R(void)> getAllArgs(lua_State* state, const std::function<R(void)>& function) {
		return function;
	}

	template<typename I, typename ...Args>
	static std::function<void(void)> getAllArgs(lua_State* state, const std::function<void(I, Args...)> function) {
		I argument = getResult<I>(state, 1);
		std::function<void(Args...)> bfunc = [function, argument](Args... args) { function(argument, args...); };
		return getAllArgs(state, bfunc);
	}

	template<typename R, typename I, typename ...Args>
	static std::function<R(void)> getAllArgs(lua_State* state, const std::function<R(I, Args...)> function) {
		I argument = getResult<I>(state, 1);
		std::function<R(Args...)> bfunc = [function, argument](Args... args) { return function(argument, args...); };
		return getAllArgs(state, bfunc);
	}

public:
	LuaScript();
	~LuaScript();

	void ExecuteFile(const std::string file);

	void CallFunction(const std::string name) {
		lua_getglobal(state, name.c_str());
		if (lua_pcall(state, 0, 0, 0)) {
			throw LuaException("function <" + name + ">", lua_tostring(state, -1));
		}
		
	}

	template<typename ...T>
	void CallFunction(const std::string name, const T... args) {
		lua_getglobal(state, name.c_str());
		int argCount = sizeof...(args);
		applyAllArgs(state, args...);
		if (lua_pcall(state, argCount, 0, 0)) {
			throw LuaException("function <" + name + ">", lua_tostring(state, -1));
		}
		return result;
	}

	template<typename R, typename ...T>
	R CallFunction(const std::string name) {
		lua_getglobal(state, name.c_str());
		if (lua_pcall(state, 0, 1, 0)) {
			throw LuaException("function <" + name + ">", lua_tostring(state, -1));
		}
		R result = getResult<R>(state, -1);
		lua_pop(state, 1);
		return result;
	}

	template<typename R, typename ...T>
	R CallFunction(const std::string name, const T... args) {
		lua_getglobal(state, name.c_str());
		int argCount = sizeof...(args);
		applyAllArgs(state, args...);
		if (lua_pcall(state, argCount, 1, 0)) {
			throw LuaException("function <" + name + ">", lua_tostring(state, -1));
		}
		R result = getResult<R>(state, -1);
		lua_pop(state, 1);
		return result;
	}

	template<typename ...Args>
	void BindFunction(const std::string fname, const std::function<void(Args...)> function) {
		std::function<int(lua_State*)> bfunc = [function](lua_State* state) {
			std::function<void(void)> func = getAllArgs(state, function);
			func();
			return 0;
		};
		lua_pushcfunction(state, &bfunc.target<int(lua_State*)>());
		lua_setglobal(state, fname.c_str());
	}

	template<typename R, typename ...Args>
	void BindFunction(const std::string fname, const std::function<R(Args...)> function) {
		std::function<int(lua_State*)> bfunc = [function](lua_State* state) {
			std::function<R(void)> func = getAllArgs(state, function);
			applyArg(state, func());
			return 1;
		};
		lua_pushcfunction(state, &bfunc.target<int(lua_State*)>());
		lua_setglobal(state, fname.c_str());
	}

	template<typename R>
	R GetGlobal(const std::string varname) {
		lua_getglobal(state, name.c_str());
		return getResult<R>(-1);
	}

	bool GlobalExists(const std::string);
};
