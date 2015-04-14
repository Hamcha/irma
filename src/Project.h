#pragma once

#include "Player.h"
#include <string>
#include <vector>

struct ProjectInfo {
	std::string name;
	std::string description;
	std::vector<std::string> scripts;
	std::vector<std::string> shaders;
};

class Project {
private:
	std::string basepath;
	std::string LoadAllShaders();
	void LoadAllScripts();

public:
	ProjectInfo info;
	Player player;

	void LoadDirectory(const std::string dirname);
	Player* CreatePlayer(const int w, const int h);
};
