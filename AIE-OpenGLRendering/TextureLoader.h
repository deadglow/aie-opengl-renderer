#pragma once
#include <string>
#include <map>
#include "glad.h"
#include "glfw3.h"
#include "Texture.h"

class TextureLoader
{
private:
	std::string dir;
	std::map<std::string, std::string> textureFiles;
	std::map<std::string, Texture*> textureLookup;

public:
	TextureLoader();
	TextureLoader(std::string dir_init);
	~TextureLoader();

	void Initialise();
	void PrintAllTextureFiles() const;

	const std::string GetTexturePath(const std::string name) const;
	const Texture* GetTexture(const std::string name) const;
	const Texture* LoadTexture(std::string filename);
	void UnloadTexture(const std::string filename);
};

