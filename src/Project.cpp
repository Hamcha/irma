#include "Project.h"
#include "FileUtils.h"

void Project::LoadDirectory(const std::string dirname) {
	basepath = dirname;

	if (!fileExists(dirname + "/project.txt")) {
		throw PlayerException("Project file is missing from " + dirname);
	}
	projectFileData raw = readProjectFile(dirname);

	if (raw.find("name") != raw.end() && raw["name"].size() > 0) {
		info.name = raw["name"][0];
	} else {
		info.name = "Unnamed project (" + dirname + ")";
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
	std::string winname = info.name + " - Player";
	player.Init(w, h, winname.c_str());

	std::string shaderCombined = LoadAllShaders();
	player.SetShader(shaderCombined);

	player.MakeUniforms();

	return &player;
}

std::string Project::LoadAllShaders() {
	std::string shaderSrc = "";
	for (auto& filename : info.shaders) {
		std::string fullname = basepath + "/" + filename;
		if (!fileExists(fullname)) {
			throw PlayerException("Inexistent shader file: " + fullname);
		}
		shaderSrc += readFileToString(fullname);
	}

	return shaderSrc;
}