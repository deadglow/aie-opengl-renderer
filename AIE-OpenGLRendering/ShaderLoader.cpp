#include "ShaderLoader.h"
#include "FileReader.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

ShaderLoader::ShaderLoader()
{
	dir = fs::current_path().string();
	shaderListFilename = "shaders.list";
}

ShaderLoader::ShaderLoader(std::string dir_init, std::string shaderListFilename_init)
{
	dir = dir_init;
	shaderListFilename = shaderListFilename_init;
}

ShaderLoader::~ShaderLoader()
{
	ClearShaders();
}

bool ShaderLoader::LoadShaders()
{
	// clear all
	ClearShaders();

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
			if (path.filename().extension().string() == ".vsd")
			{
				vertexShaderFiles.push_back(path);
				continue;
			}
			if (path.filename().extension().string() == ".fsd")
			{
				fragmentShaderFiles.push_back(path);
				continue;
			}
		}
	}

	std::cout << vertexShaderFiles.size() << " vertex shaders, " << fragmentShaderFiles.size() << " fragment shaders." << std::endl;

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
			std::cout << "Fragment shader " << vertexShaderFiles[i].filename().string() << " failed:" << std::endl;
			glGetShaderInfoLog(shader, 512, nullptr, errorLog);
			std::cout << errorLog << std::endl;
			shaderStateOkay = false;
		}
		else
		{
			std::cout << "Fragment shader " << vertexShaderFiles[i].filename().string() << " compiled." << std::endl;
		}

		fragmentShaders.emplace(fragmentShaderFiles[i].filename().string(), shader);
	}

	struct ShaderProgramStrings
	{
		std::string name = "";
		std::string vertex = "";
		std::string fragment = "";
	};

	std::vector<ShaderProgramStrings> programStrings;

	// go through shader program list and generate program pairs
	std::ifstream file(shaderListFile.string());
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::string thisLine;
			std::getline(file, thisLine);

			ShaderProgramStrings strings;
			int word = 0;
			// construct name, vertex and fragment strings. go to the next word when a comma is found.
			for (char c : thisLine)
			{
				// newline, gettouta there
				if (c == '\n') break;
				if (c == ',')
				{
					word++;
					continue;
				}

				switch (word)
				{
				case 0:
					strings.name.push_back(c);
					break;
				case 1:
					strings.vertex.push_back(c);
					break;
				case 2:
					strings.fragment.push_back(c);
					break;
				default:
					break;
				}
			}

			// get rid of trailing spaces??
			programStrings.push_back(strings);
		}
	}
	else
	{
		std::cout << "Error loading shader list. Path: " << shaderListFile.string() << std::endl;
		shaderStateOkay = false;
	}
	file.close();

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
				std::cout << "Successfully linked shader program " << programStrings[i].name << std::endl;
			}

			shaderPrograms.emplace(programStrings[i].name, program);
		}

	}

	return shaderStateOkay;
}

void ShaderLoader::UseShader(GLuint shader)
{
	currentShader = shader;
	glUseProgram(currentShader);
}

void ShaderLoader::ClearShaders()
{
	for (const auto& [key, value] : shaderPrograms)
	{
		glDeleteProgram(value);
	}
	for (const auto& [key, value] : vertexShaders)
	{
		glDeleteShader(value);
	}
	for (const auto& [key, value] : fragmentShaders)
	{
		glDeleteShader(value);
	}

	shaderPrograms.clear();
	vertexShaders.clear();
	fragmentShaders.clear();
}

void ShaderLoader::UseShader(std::string shader)
{
	if (shaderPrograms.count(shader))
	{
		UseShader(shaderPrograms[shader]);
		currentShaderName = shader;
	}
}

void ShaderLoader::PrintShaderCollections()
{
	std::cout << "\n-----------------------\nVertex shaders:" << std::endl;
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
		std::cout << key << ", " << value << std::endl;
	}
}

GLuint ShaderLoader::GetUniformLocation(std::string variable)
{
	return GLuint();
}

void ShaderLoader::SetUniform(std::string variable, float value)
{
}

void ShaderLoader::SetUniform(std::string variable, glm::mat4 value)
{
}
