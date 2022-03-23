#pragma once
#include <string>
#include <unordered_map>
#include "glad.h"
#include "GLFW/glfw3.h"
#include "Texture.h"

class TextureLoader
{
private:
	static std::string dir;
	static std::unordered_map<std::string, std::string> textureFiles;
	static std::unordered_map<std::string, Texture*> textureLookup;

	TextureLoader() = delete;
public:

	static void Initialise();
	static void Shutdown();
	static void PrintAllTextureFiles();

	static const std::string GetTexturePath(const std::string filename);
	static Texture* GetTexture(const std::string filename);
	static Texture* LoadTexture(const std::string filename);
	static void UnloadTexture(const std::string filename);
};

