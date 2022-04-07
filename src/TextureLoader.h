#pragma once
#include <string>
#include <unordered_map>
#include "glad.h"
#include "GLFW/glfw3.h"
#include "Texture2D.h"
#include "Cubemap.h"

class TextureLoader
{
private:
	static std::string dir;
	static std::unordered_map<std::string, std::string> textureFiles;

	TextureLoader() = delete;

	static void LoadTexture2D(Texture2D* tex);
	static void LoadCubemap(Cubemap* tex);
public:
	static std::unordered_map<std::string, Texture*> textureLookup;
	static void Initialise();
	static void Shutdown();
	static void PrintAllTextureFiles();

	static const std::string GetTexturePath(const std::string filename);
	static Texture2D* GetTexture(const std::string filename);
	static Cubemap* GetCubemap(const std::string filename);
	static void LoadTexture(Texture* tex);
	static void UnloadTexture(Texture* tex);

	static void AddTexture(Texture* tex);
	static void RemoveTexture(const std::string filename);
};

