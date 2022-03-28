#pragma once
#include <string>
#include <unordered_map>
#include "glad.h"
#include "GLFW/glfw3.h"
#include "Cubemap.h"

class TextureLoader
{
private:
	static std::string dir;
	static std::unordered_map<std::string, std::string> textureFiles;
	static std::unordered_map<std::string, Texture2D*> textureLookup;
	static std::unordered_map<std::string, Cubemap*> cubemapLookup;

	TextureLoader() = delete;
public:

	static void Initialise();
	static void Shutdown();
	static void PrintAllTextureFiles();

	static const std::string GetTexturePath(const std::string filename);
	static Texture2D* GetTexture(const std::string filename);
	static Cubemap* GetCubemap(const std::string filename);
	static Texture2D* LoadTexture(const std::string filename);
	static Cubemap* LoadCubemap(const std::string faceFilenames[6]);
	static void UnloadTexture(const std::string filename);
};

