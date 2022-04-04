#include "PostProcessingStack.h"

PostProcessingStack::PostProcessingStack(const std::string name_init, std::vector<std::string> list_init)
{
	name = name_init;
	shaderList = list_init;
}