#include "LuaFunctions.h"
#include <functional>

template<typename T>
void _lua_SetUniform(Player* player, const char* name, T value) {
	auto* uniforms = &player->userShader.uniforms;
	std::string strname = std::string(name);
	if (uniforms->find(strname) == uniforms->end()) {
		uniforms->at(strname) = new Uniform<T>(value);
	} else {
		auto typedUniform = (Uniform<T>*)uniforms->at(strname);
		typedUniform->value = value;
	}
}

void bindLuaFunctions(LuaScript* script, Player* player) {
	std::function<void(const char*, float)> SetUniformFloat = std::bind(_lua_SetUniform<float>, player);
	script->bindFunction("uniformFloat", SetUniformFloat);

	std::function<void(const char*, double)> SetUniformDouble = std::bind(_lua_SetUniform<double>, player);
	script->bindFunction("uniformDouble", SetUniformDouble);

	std::function<void(const char*, int)> SetUniformInt = std::bind(_lua_SetUniform<int>, player);
	script->bindFunction("uniformInt", SetUniformInt);
}