#pragma once

#include <lua.hpp>
#include <string>
#include <stdexcept>
#include <functional>

template<typename R>
void applyArg(lua_State*, const R);

template<typename R>
R getResult(lua_State* state, int index);

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

	static std::function<void(void)> getAllArgs(lua_State* state, const std::function<void(void)>& function, int index) {
		return function;
	}

	template<typename R>
	static std::function<R(void)> getAllArgs(lua_State* state, const std::function<R(void)>& function, int index) {
		return function;
	}

	template<typename I, typename ...Args>
	static std::function<void(void)> getAllArgs(lua_State* state, const std::function<void(I, Args...)> function, int index) {
		I argument = getResult<I>(state, index);
		std::function<void(Args...)> bfunc = [function, argument](Args... args) { function(argument, args...); };
		return getAllArgs(state, bfunc, index + 1);
	}

	template<typename R, typename I, typename ...Args>
	static std::function<R(void)> getAllArgs(lua_State* state, const std::function<R(I, Args...)> function, int index) {
		I argument = getResult<I>(state, index);
		std::function<R(Args...)> bfunc = [function, argument](Args... args) { return function(argument, args...); };
		return getAllArgs(state, bfunc, index + 1);
	}

public:
	LuaScript();
	~LuaScript();

	void ExecuteFile(const std::string file);

	template<typename R, typename ...T>
	R CallFunction(const std::string name, const T... args) {
		lua_getglobal(state, name.c_str());
		int argCount = sizeof...(args);
		applyAllArgs(state, args...);
		lua_pcall(state, argCount, 1, 0);

		R result = getResult<R>(state, -1);
		lua_pop(state, 1);
		return result;
	}

	template<typename ...Args>
	void BindFunction(const std::string fname, const std::function<void(Args...)> function) {
		std::function<int(lua_State*)> bfunc = [function](lua_State* state) {
			std::function<void(void)> func = getAllArgs(state, function, 1);
			func();
			return 0;
		};
		lua_pushcfunction(state, &bfunc.target<int(lua_State*)>());
		lua_setglobal(state, fname.c_str());
	}

	template<typename R, typename ...Args>
	void BindFunction(const std::string fname, const std::function<R(Args...)> function) {
		std::function<int(lua_State*)> bfunc = [function](lua_State* state) {
			std::function<R(void)> func = getAllArgs(state, function, 1);
			applyArg(state, func());
			return 1;
		};
		lua_pushcfunction(state, &bfunc.target<int(lua_State*)>());
		lua_setglobal(state, fname.c_str());
	}
};


class LuaException : public std::runtime_error {
public:
	std::string file;
	LuaException(std::string filename, std::string what) : std::runtime_error(what.c_str()) { file = filename; }
};
