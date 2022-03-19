#pragma once
#include <string>
#include <map>

class TextureLoader
{
private:
	std::string dir;
	std::map<std::string, std::string> textureFiles;
	std::map<std::string, unsigned int> textureLookup;

public:
	TextureLoader();
	TextureLoader(std::string dir_init);
	~TextureLoader();

	void Initialise();
	void PrintAllTextureFiles() const;

	const std::string GetTexturePath(const std::string name) const;
	const unsigned int GetTextureID(const std::string name) const;
	const unsigned int LoadTexture(std::string filename);
	void UnloadTexture(const std::string filename);
};

