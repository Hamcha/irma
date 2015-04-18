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
	std::function<void(const char*, float)> SetUniformFloat = std::bind(_lua_SetUniform<float>, player,
		std::placeholders::_1, std::placeholders::_2);
	script->BindFunction("SetFloat", SetUniformFloat);
	std::function<void(const char*, double)> SetUniformDouble = std::bind(_lua_SetUniform<double>, player,
		std::placeholders::_1, std::placeholders::_2);
	script->BindFunction("SetDouble", SetUniformDouble);

	std::function<void(const char*, int)> SetUniformInt = std::bind(_lua_SetUniform<int>, player,
		std::placeholders::_1, std::placeholders::_2);
	script->BindFunction("SetInt", SetUniformInt);

	script->BindFunction("AddNum", std::function<int(int, int)>([](int a, int b) { return a + b; }));
}