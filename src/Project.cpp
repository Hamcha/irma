#include "Project.h"
#include "FileUtils.h"

Project Project::FromDirectory(const std::string dirname) {
	Project project(dirname);
	return project;
}

Project::Project(const std::string dirname) {
	basepath = dirname;
	projectFileData raw = readProjectFile(dirname);
	if (raw.find("name") != raw.end() && raw["name"].size() > 0) {
		info.name = raw["name"][0];
	}
	if (raw.find("description") != raw.end() && raw["description"].size() > 0) {
		for (auto& line : raw["description"]) {
			info.description += line + "\n";
		}
	}
	if (raw.find("scripts") != raw.end() && raw["scripts"].size() > 0) {
		info.scripts = raw["scripts"];
	}
	if (raw.find("shaders") != raw.end() && raw["shaders"].size() > 0) {
		info.shaders = raw["shaders"];
	}
}

Player* Project::CreatePlayer(const int w, const int h) {
	player.Init(w, h);

	std::string shaderCombined = LoadAllShaders();
	player.SetShader(shaderCombined);

	player.MakeUniforms();

	return &player;
}

std::string Project::LoadAllShaders() {
	std::string shaderSrc = "";
	for (auto& filename : info.shaders) {
		shaderSrc += readFileToString(basepath + "/" + filename);
	}

	return shaderSrc;
}

void Project::Dispose() {
	player.Dispose();
}