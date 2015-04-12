#include "FileUtils.h"
#include <fstream>

std::string readFileToString(std::string path) {
	std::ifstream file(path); //TODO Check for existence
	std::string out = "", line;
	while (!file.eof()) {
		std::getline(file, line);
		out += line + "\n";
	}
	file.close();
	return out;
}

projectFileData readProjectFile(std::string path) {
	projectFileData data;
	std::ifstream file(path+"/project.txt");
	std::string line, currentField;
	while (!file.eof()) {
		std::getline(file, line);
		if (line.length() < 1) continue;

		// Trim string (and continue if empty)
		std::size_t start = line.find_first_not_of(" \t\f\v\n\r");
		if (start == std::string::npos) continue;
		line.erase(0, start);

		// Set as current key if not indented, add to current fields otherwise
		if (start > 0) {
			data[currentField].push_back(line);
		} else {
			currentField = line;
		}
	}
	file.close();
	return data;
}