#include "PostProcessing.h"
#include "FileReader.h"
#include "rapidjson/document.h"
#include <filesystem>

using namespace rapidjson;

std::string PostProcessing::dir = fs::current_path().string() + "\\shaders\\postprocessing.json";
std::unordered_map<std::string, PostProcessingStack*> PostProcessing::stacks;


void PostProcessing::Initialise()
{
	PostProcessingStack* stack = LoadPostProcessing(dir);
	stacks.emplace(stack->name, stack);
}

void PostProcessing::Shutdown()
{
	for (const auto [key, value] : stacks)
	{
		delete value;
	}
}

PostProcessingStack* PostProcessing::LoadPostProcessing(std::string path)
{
	std::string contents = FileReader::LoadFileAsString(path);
	Document doc;
	doc.Parse(contents.c_str());

	assert(doc.IsObject());

	std::string name = doc["name"].GetString();

	const Value& stack = doc["stack"];

	assert(stack.IsArray());

	std::vector<std::string> shaderNames;
	for (int i = 0; i < stack.Size(); ++i)
	{
		shaderNames.push_back(stack[i].GetString());
	}

	PostProcessingStack* newStack = new PostProcessingStack(name, shaderNames);

	return newStack;
}
