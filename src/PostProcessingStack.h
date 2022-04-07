#pragma once
#include <vector>
#include <string>

class PostProcessingStack
{
private:

public:
	std::string name;
	std::vector<std::string> shaderList;

	PostProcessingStack(const std::string name_init, std::vector<std::string> list_init);
};

