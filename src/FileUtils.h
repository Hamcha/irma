#ifndef _FILEUTILS_H
#define _FILEUTILS_H

#include <string>
#include <map>
#include <vector>

typedef std::map<std::string, std::vector<std::string>> projectFileData;

bool fileExists(std::string path);
std::string readFileToString(std::string path);
projectFileData readProjectFile(std::string path);

#endif