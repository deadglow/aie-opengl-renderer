#include "ShaderLoader.h"
#include "FileReader.h"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "rapidjson/document.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;
namespace rj = rapidjson;

std::string ShaderLoader::dir = fs::current_path().string() + "\\shaders";
std::string ShaderLoader::shaderListFilename = "shaders.json";
std::unordered_map<std::string, unsigned int> ShaderLoader::vertexShaders;
std::unordered_map<std::string, unsigned int> ShaderLoader::fragmentShaders;
std::unordered_map<std::string, Shader*> ShaderLoader::shaderPrograms;

bool ShaderLoader::shaderStateOkay = false;
Shader* ShaderLoader::currentShader = nullptr;


void ShaderLoader::Shutdown()
{
	ClearShaders();
}

struct ShaderProgramStrings
{
	std::string name = "";
	std::string vertex = "";
	std::string fragment = "";
};

bool ShaderLoader::LoadInShaders()
{
	shaderStateOkay = true;

	std::vector<fs::path> vertexShaderFiles;
	std::vector<fs::path> fragmentShaderFiles;
	fs::path shaderListFile;

	// find all vetex and fragment shader files, and the shader list
	if (fs::exists(dir) && fs::is_directory(dir))
	{
		for (fs::directory_entry const& entry : fs::recursive_directory_iterator(dir))
		{
			if (!fs::is_regular_file(entry)) continue;

			fs::path path = entry.path();
			if (path.filename().string() == shaderListFilename)
			{
				shaderListFile = path;
				continue;
			}
			if (path.filename().extension().string() == ".vert")
			{
				vertexShaderFiles.push_back(path);
				continue;
			}
			if (path.filename().extension().string() == ".frag")
			{
				fragmentShaderFiles.push_back(path);
				continue;
			}
		}
	}

	std::cout << "Found " << vertexShaderFiles.size() << " vertex shaders, " << fragmentShaderFiles.size() << " fragment shaders." << std::endl;

	// generate vertex shader collection
	for (int i = 0; i < vertexShaderFiles.size(); ++i)
	{
		const std::string contentsStr = FileReader::LoadFileAsString(vertexShaderFiles[i].string());
		const char* contents = contentsStr.c_str();
		// create shader handle
		GLuint shader = glCreateShader(GL_VERTEX_SHADER);
		// populate source with code
		glShaderSource(shader, 1, &contents, nullptr);
		// compile code
		glCompileShader(shader);

		// verify if compilation was successful
		GLchar errorLog[512];
		GLint success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			// something failed with the vertex shader compilation
			std::cout << "Vertex shader " << vertexShaderFiles[i].filename().string() << " failed:" << std::endl;
			glGetShaderInfoLog(shader, 512, nullptr, errorLog);
			std::cout << errorLog << std::endl;
			shaderStateOkay = false;
		}
		else
		{
			std::cout << "Vertex shader " << vertexShaderFiles[i].filename().string() << " compiled." << std::endl;
		}
		
		vertexShaders.emplace(vertexShaderFiles[i].filename().string(), shader);
	}

	// generate fragment shader collection
	for (int i = 0; i < fragmentShaderFiles.size(); ++i)
	{
		const std::string contentsStr = FileReader::LoadFileAsString(fragmentShaderFiles[i].string());
		const char* contents = contentsStr.c_str();
		// create shader handle
		GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
		// populate source with code
		glShaderSource(shader, 1, &contents, nullptr);
		// compile code
		glCompileShader(shader);

		// verify if compilation was successful
		GLchar errorLog[512];
		GLint success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			// something failed with the vertex shader compilation
			std::cout << "Fragment shader " << fragmentShaderFiles[i].filename().string() << " failed:" << std::endl;
			glGetShaderInfoLog(shader, 512, nullptr, errorLog);
			std::cout << errorLog << std::endl;
			shaderStateOkay = false;
		}
		else
		{
			std::cout << "Fragment shader " << fragmentShaderFiles[i].filename().string() << " compiled." << std::endl;
		}

		fragmentShaders.emplace(fragmentShaderFiles[i].filename().string(), shader);
	}

	std::vector<ShaderProgramStrings> programStrings;

	// go through shader program list and generate program pairs
	std::string contents = FileReader::LoadFileAsString(shaderListFile.string());
	rj::Document doc;
	doc.Parse(contents.c_str());
	
	assert(doc.IsObject());

	const rj::Value& shaderArray = doc["shaders"];
	assert(shaderArray.IsArray());

	programStrings.resize(shaderArray.Size());
	for (int i = 0; i < shaderArray.Size(); ++i)
	{
		programStrings[i].name = shaderArray[i]["name"].GetString();
		if (shaderArray[i].HasMember("vert"))
			programStrings[i].vertex = shaderArray[i]["vert"].GetString();
		if (shaderArray[i].HasMember("frag"))
			programStrings[i].fragment = shaderArray[i]["frag"].GetString();
	}

	if (shaderStateOkay)
	{
		for (int i = 0; i < programStrings.size(); ++i)
		{
			GLuint program = glCreateProgram();
			glAttachShader(program, fragmentShaders[programStrings[i].fragment]);
			glAttachShader(program, vertexShaders[programStrings[i].vertex]);
			glLinkProgram(program);

			GLchar errorLog[512];
			GLint success = 0;
			glGetProgramiv(program, GL_LINK_STATUS, &success);
			if (success == GL_FALSE)
			{
				std::cout << "Linking error with program " << programStrings[i].name << std::endl;
				glGetProgramInfoLog(program, 512, nullptr, errorLog);
				std::cout << errorLog << std::endl;
				shaderStateOkay = false;
			}
			else
			{
				Shader* newShader = new Shader(program, programStrings[i].name);
				shaderPrograms.emplace(newShader->GetName(), newShader);
				std::cout << "Successfully linked shader program " << programStrings[i].name << std::endl;
			}
		}
	}

	return shaderStateOkay;
}

void ShaderLoader::DeleteVertexAndFragmentShaders()
{
	for (const auto& [key, value] : vertexShaders)
	{
		glDeleteShader(value);
	}
	for (const auto& [key, value] : fragmentShaders)
	{
		glDeleteShader(value);
	}
	vertexShaders.clear();
	fragmentShaders.clear();

	std::cout << "Vertex and Fragment shaders deleted.\n" << std::endl;
}


void ShaderLoader::ClearShaders()
{
	for (const auto& [key, value] : shaderPrograms)
	{
		delete value;
	}

	shaderPrograms.clear();

	DeleteVertexAndFragmentShaders();
	std::cout << "All shaders cleared.\n" << std::endl;
}

const bool ShaderLoader::InitialiseShaders()
{
	ClearShaders();
	if (LoadInShaders())
	{
		PrintShaderCollections();
		DeleteVertexAndFragmentShaders();
		return true;
	}
	else
		return false;
}

Shader* ShaderLoader::GetCurrentShader()
{
	return currentShader;
}

Shader* ShaderLoader::GetShader(const std::string filename)
{
	return shaderPrograms.at(filename);
}

void ShaderLoader::UseShader(Shader* shader)
{
	currentShader = shader;
	glUseProgram(shader->GetID());
}

void ShaderLoader::UseShader(const std::string shader)
{
	if (shaderPrograms.count(shader))
	{
		UseShader(shaderPrograms[shader]);
	}
}

void ShaderLoader::PrintShaderCollections()
{
	std::cout << "\n____Start Shader list____\n-----------------------\nVertex shaders:" << std::endl;
	for (auto const [key, value] : vertexShaders)
	{
		std::cout << key << ", " << value << std::endl;
	}
	std::cout << "\n-----------------------\nFragment shaders:" << std::endl;
	for (auto const [key, value] : fragmentShaders)
	{
		std::cout << key << ", " << value << std::endl;
	}
	std::cout << "\n-----------------------\nShader programs:" << std::endl;
	for (auto const [key, value] : shaderPrograms)
	{
		std::cout << key << ", " << value->GetID() << std::endl;
	}

	std::cout << "\n----End Shader List----\n\n";
}
