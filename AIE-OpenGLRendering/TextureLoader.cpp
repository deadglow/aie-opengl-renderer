#include "TextureLoader.h"
#include "stb_image.h"
#include "glad.h"
#include "glfw3.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

TextureLoader::TextureLoader()
{
	dir = fs::current_path().string() + "/textures";
}

TextureLoader::TextureLoader(std::string dir_init)
{
	dir = dir_init;
}

TextureLoader::~TextureLoader()
{
	for (auto const [key, value] : textureLookup)
	{
		glDeleteTextures(1, &value);
	}
	textureLookup.clear();
	std::cout << "Textures unloaded." << std::endl;
}

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
			if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".tga")
			{
				textureFiles.emplace(path.filename().string(), path.string());
				continue;
			}
		}
	}
}

void TextureLoader::PrintAllTextureFiles() const
{
	std::cout << "Texture files: " << std::endl;
	for (auto const [key, value] : textureFiles)
	{
		std::cout << key << "\n" << std::endl;
	}
}

const std::string TextureLoader::GetTexturePath(const std::string name) const
{
	return textureFiles.at(name);
}

const unsigned int TextureLoader::GetTextureID(const std::string name) const
{
	return textureLookup.at(name);
}

const unsigned int TextureLoader::LoadTexture(std::string filename)
{
	if (textureFiles.count(filename))
	{
		// load image data
		std::string path = GetTexturePath(filename);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			// set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// assign texture data
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

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
			return -1;
		}
	}
	else
		return -1;
}

void TextureLoader::UnloadTexture(const std::string filename)
{
	if (textureLookup.count(filename))
	{
		unsigned int id = textureLookup[filename];
		glDeleteTextures(1, &id);

		textureLookup.erase(filename);
		std::cout << "Unloaded texture " << filename << std::endl;
	}
	else
	{
		std::cout << "Texture " << filename << " not loaded." << std::endl;
	}
}
