#pragma once
#include <string>
#include <unordered_map>
#include "PostProcessingStack.h"

class PostProcessing
{
private:
	static std::string dir;
	

	PostProcessing() = delete;
public:
	static std::unordered_map<std::string, PostProcessingStack*> stacks;

	static void Initialise();
	static void DeleteAll();
	static void Shutdown();

	static PostProcessingStack* LoadPostProcessing(std::string path);
};
