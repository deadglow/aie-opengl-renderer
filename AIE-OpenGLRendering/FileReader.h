#pragma once
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class FileReader
{
public:
	static std::string LoadFileAsString(std::string filename);
	static int LoadFileAsStringVector(std::vector<std::string>* lines, const std::string filename);
	static int GetAllFilesWithExtension(std::string dir, std::string extension, std::vector<fs::path>* paths);
};

