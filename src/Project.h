#ifndef _PROJECT_H
#define _PROJECT_H

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

	Project(const std::string dirname);
	std::string LoadAllShaders();

public:
	ProjectInfo info;
	Player player;

	static Project FromDirectory(const std::string dirname);

	Player* CreatePlayer(const int w, const int h);
	void Dispose();
};

#endif