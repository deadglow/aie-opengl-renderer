#include "TextureLoader.h"
#include "stb_image.h"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "FileReader.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::string TextureLoader::dir = fs::current_path().string() + "\\textures";;
std::unordered_map<std::string, std::string> TextureLoader::textureFiles;
std::unordered_map<std::string, Texture*> TextureLoader::textureLookup;




void TextureLoader::Initialise()
{
	std::vector<fs::path> paths;
	std::vector<std::string> extensions{ ".png", ".jpg", ".jpeg", ".tga", ".cubemap", ".hdr" };

	// find all texture files in the textures folder
	FileReader::GetAllFilesWithExtensions(dir, &extensions, &paths);

	for (int i = 0; i < paths.size(); ++i)
	{
		std::string filename = paths[i].filename().string();
		textureFiles.emplace(filename, paths[i].string());
		
		if (paths[i].extension().string() == ".cubemap")
		{
			// create cubemap
			Cubemap* cubemap = new Cubemap(filename);
			textureLookup.emplace(filename, cubemap);
		}
		else
		{
			Texture2D* tex = new Texture2D(filename);
			textureLookup.emplace(filename, tex);
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

void TextureLoader::LoadTexture(Texture* tex)
{
	// fail if already loaded
	if (tex->IsLoaded())
		return;

	switch (tex->GetTexType())
	{
	case TEX_Type::Texture2D:
		LoadTexture2D((Texture2D*)tex);
		break;
	case TEX_Type::Cubemap:
		LoadCubemap((Cubemap*)tex);
		break;
	default:
		break;
	}
}

void TextureLoader::LoadTexture(const std::string name)
{
	LoadTexture(textureLookup.at(name));
}

void TextureLoader::LoadTexture2D(Texture2D* tex)
{
	stbi_set_flip_vertically_on_load(true);
	// load image data
	std::string path = GetTexturePath(tex->GetFilename());
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

		tex->SetID(id);
		tex->SetProperties(width, height, format);

		// we're now done with the data
		stbi_image_free(data);

		std::cout << "Successfully loaded texture " << tex->GetFilename() << std::endl;
	}
	else
	{
		stbi_image_free(data);
		std::cout << "Failed to load image from disk." << std::endl;
	}
}

void TextureLoader::LoadCubemap(Cubemap* tex)
{
	std::vector<std::string> lines;
	std::string pathString = GetTexturePath(tex->GetFilename());

	// cancel if not enough lines are read
	if (FileReader::LoadFileAsStringVector(&lines, pathString) < 1) return;

	// check if cubemap is hdr or not
	bool isHDR = lines[0] == "true";
	GLenum format = isHDR ? GL_RGB16F : GL_RGB;
	GLenum dataType = isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE;

	std::string faceTextures[6];
	int lastValidLine = 1;
	for (int i = 1; i < 7; ++i)
	{
		if (i >= lines.size() || lines[i].size() <= 2)
		{
			std::cout << "Not enough textures for cubemap, filling with last texture." << std::endl;
			faceTextures[i - 1] = faceTextures[lastValidLine];
		}
		else
		{
			faceTextures[i - 1] = lines[i];
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
		
		fs::path texPath = path.string() + "\\" + faceTextures[i];
		void* data;
		if (isHDR)
			data = stbi_loadf(texPath.string().c_str(), &width, &height, &nrChannels, 0);
		else
			data = stbi_load(texPath.string().c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, GL_RGB, dataType, data);

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
	
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	tex->SetID(id);
	tex->UpdateTextureProperties();
	std::cout << "Loaded cubemap: " << tex->GetFilename() << std::endl;
}

void TextureLoader::UnloadTexture(Texture* tex)
{
	// texture already unloaded?
	if (!tex->IsLoaded()) return;

	GLuint id = tex->GetID();
	glDeleteTextures(1, &id);
	tex->SetID(-1);

	switch (tex->GetTexType())
	{
	case TEX_Type::Texture2D:
		std::cout << "Unloaded texture: " << tex->GetFilename() << std::endl;
		break;
	case TEX_Type::Cubemap:
		std::cout << "Unloaded cubemap: " << tex->GetFilename() << std::endl;
		break;
	default:
		break;
	}
}

void TextureLoader::AddTexture(Texture* tex)
{
	textureLookup.emplace(tex->GetFilename(), tex);
}

void TextureLoader::RemoveTexture(const std::string filename)
{
	textureLookup.erase(filename);
}
