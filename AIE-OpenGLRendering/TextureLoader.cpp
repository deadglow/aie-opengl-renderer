#include "TextureLoader.h"
#include "stb_image.h"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "FileReader.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::string TextureLoader::dir = fs::current_path().string() + "/textures";;
std::unordered_map<std::string, std::string> TextureLoader::textureFiles;
std::unordered_map<std::string, std::string> TextureLoader::cubemapFiles;
std::unordered_map<std::string, Texture*> TextureLoader::textureLookup;

void TextureLoader::Initialise()
{
	// find all texture files in the textures folder
	if (fs::exists(dir) && fs::is_directory(dir))
	{
		for (fs::directory_entry const& entry : fs::recursive_directory_iterator(dir))
		{
			if (!fs::is_regular_file(entry)) continue;

			fs::path path = entry.path();
			std::string extension = path.filename().extension().string();
			if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".tga" || extension == ".cubemap")
			{
				textureFiles.emplace(path.filename().string(), path.string());
				continue;
			}
		}
	}
}

void TextureLoader::Shutdown()
{
	for (auto const [key, value] : textureLookup)
	{
		delete value;
	}
	textureLookup.clear();
	std::cout << "Textures unloaded." << std::endl;
}

void TextureLoader::PrintAllTextureFiles()
{
	std::cout << "Texture files: " << std::endl;
	for (auto const [key, value] : textureFiles)
	{
		std::cout << key << "\n" << std::endl;
	}
}

const std::string TextureLoader::GetTexturePath(const std::string filename)
{
	return textureFiles.at(filename);
}

Texture2D* TextureLoader::GetTexture(const std::string filename)
{
	return (Texture2D*)textureLookup.at(filename);
}

Cubemap* TextureLoader::GetCubemap(const std::string filename)
{
	return (Cubemap*)textureLookup.at(filename);
}

Texture2D* TextureLoader::LoadTexture(const std::string filename)
{
	// fail if texture isn't found
	if (!textureFiles.count(filename)) return nullptr;

	stbi_set_flip_vertically_on_load(true);
	// load image data
	std::string path = GetTexturePath(filename);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		unsigned int id;
		// assign texture data
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		TEX_Format format = Texture2D::CalculateFormat(nrChannels);
		glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)format, width, height, 0, (GLenum)format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		Texture2D* texture = new Texture2D(id, filename, width, height, format);

		// we're now done with the data
		stbi_image_free(data);

		// add to the texture lookup
		textureLookup.emplace(filename, texture);

		std::cout << "Successfully loaded texture " << filename << std::endl;
		return texture;
	}
	else
	{
		stbi_image_free(data);
		std::cout << "Failed to load image from disk." << std::endl;
		return nullptr;
	}
}

Cubemap* TextureLoader::LoadCubemap(const std::string filename)
{
	std::vector<std::string> lines;
	std::string pathString = GetTexturePath(filename);
	
	// cancel if not enough lines are read
	if (FileReader::LoadFileAsStringVector(&lines, pathString) < 1) return nullptr;

	std::string faceTextures[6];
	int lastValidLine = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (i >= lines.size() || lines[i].size() <= 1)
		{
			std::cout << "Not enough textures for cubemap, filling with last texture." << std::endl;
			faceTextures[i] = faceTextures[lastValidLine];
		}
		else
		{
			faceTextures[i] = lines[i];
			lastValidLine = i;
		}
	}

	fs::path path = pathString;
	path = path.parent_path();

	unsigned int id = -1;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	int width, height, nrChannels;
	// right, left, top, bottom, back, front
	for (unsigned int i = 0; i < 6; ++i)
	{
		stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load((path.string() + "/" + faceTextures[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			GLenum format = (GLenum)Texture2D::CalculateFormat(nrChannels);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap failed to load path: " + faceTextures[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	Cubemap* cubemap = new Cubemap(id, filename);

	textureLookup.emplace(filename, cubemap);
	
	return cubemap;
	//---------------------------------------------------------------------------------------------
}

void TextureLoader::UnloadTexture(const std::string filename)
{
	if (textureLookup.count(filename))
	{
		delete textureLookup[filename];

		textureLookup.erase(filename);
		std::cout << "Unloaded texture " << filename << std::endl;
	}
	else
	{
		std::cout << "Texture " << filename << " not loaded." << std::endl;
	}
}
