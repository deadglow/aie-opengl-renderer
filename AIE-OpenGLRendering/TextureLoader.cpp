#include "TextureLoader.h"
#include "stb_image.h"
#include "glad.h"
#include "glfw3.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::string TextureLoader::dir = fs::current_path().string() + "/textures";;
std::unordered_map<std::string, std::string> TextureLoader::textureFiles;
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
			if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".tga")
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

const Texture* TextureLoader::GetTexture(const std::string filename)
{
	return textureLookup.at(filename);
}

const Texture* TextureLoader::LoadTexture(const std::string filename)
{
	if (textureFiles.count(filename))
	{
		// load image data
		std::string path = GetTexturePath(filename);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			Texture* texture = new Texture(data, width, height, nrChannels, filename);

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
	else
		return nullptr;
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
