#include "ShaderLoader.h"
#include <filesystem>
#include <vector>
#include <iostream>
#include "FileReader.h"

namespace fs = std::filesystem;


ShaderLoader::ShaderLoader()
{
    dir = "";
    shaderListFilename = "shaders.list";
}

ShaderLoader::ShaderLoader(std::string dir_init, std::string shaderListFilename_init)
{
    dir = dir_init;
    shaderListFilename = shaderListFilename_init;
}

ShaderLoader::~ShaderLoader()
{
}

bool ShaderLoader::LoadShaders()
{
    shaderStateOkay = true;
    // clear all
    ClearShaders();

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
            if (path.filename() == shaderListFilename)
            {
                shaderListFile = path;
                continue;
            }
            if (path.extension() == "vsd")
            {
                vertexShaderFiles.push_back(path);
                continue;
            }
            if (path.extension() == "fsd")
            {
                fragmentShaderFiles.push_back(path);
                continue;
            }
        }
    }

    // generate vertex shader collection
    for (int i = 0; i < vertexShaderFiles.size(); ++i)
    {
        const char* contents = FileReader::LoadFileAsString(vertexShaderFiles[i].string()).c_str();
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
            std::cout << "Vertex shader " << vertexShaderFiles[i].filename() << " failed:" << std::endl;
            glGetShaderInfoLog(shader, 512, nullptr, errorLog);
            std::cout << errorLog << std::endl;
            shaderStateOkay = false;
        }
        else
        {
            std::cout << "Vertex shader " << vertexShaderFiles[i].filename() << " compiled." << std::endl;
        }
    }

    // generate fragment shader collection
    for (int i = 0; i < fragmentShaderFiles.size(); ++i)
    {
        const char* contents = FileReader::LoadFileAsString(fragmentShaderFiles[i].string()).c_str();
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
            std::cout << "Fragment shader " << vertexShaderFiles[i].filename() << " failed:" << std::endl;
            glGetShaderInfoLog(shader, 512, nullptr, errorLog);
            std::cout << errorLog << std::endl;
            shaderStateOkay = false;
        }
        else
        {
            std::cout << "Fragment shader " << vertexShaderFiles[i].filename() << " compiled." << std::endl;
        }
    }

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
