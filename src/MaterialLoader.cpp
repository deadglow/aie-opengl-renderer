#include "MaterialLoader.h"
#include "FileReader.h"
#include "rapidjson/document.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include <iostream>

using namespace rapidjson;

std::string MaterialLoader::dir = fs::current_path().string() + "\\materials";
std::unordered_map<std::string, Material*> MaterialLoader::materialLookup;

void MaterialLoader::Initialise()
{
	std::vector<fs::path> paths;
	std::vector<std::string> extensions{ ".mat" };

	// find all mesh files in the meshes folder
	FileReader::GetAllFilesWithExtensions(dir, &extensions, &paths);

	for (int i = 0; i < paths.size(); ++i)
	{
		// read material file and parse it here
		Material* material = BuildMaterialFromFile(paths[i].string());
		materialLookup.emplace(material->GetName(), material);
	}
}

void MaterialLoader::Shutdown()
{
	for (const auto [key, value] : materialLookup)
	{
		delete value;
	}
}

void MaterialLoader::PrintAllMaterials()
{
	for (const auto [key, value] : materialLookup)
	{
		std::cout << key << std::endl;
	}
}

Material* MaterialLoader::CloneMaterial(Material* material, std::string name)
{
	Material* mat = new Material(material->GetShader(), name);
	for (int i = 0; i < material->usedTextures.size(); ++i)
	{
		mat->AddTexture(material->usedTextures[i]);
	}

	materialLookup.emplace(mat->GetName(), mat);

	return mat;
}

Material* MaterialLoader::BuildMaterialFromFile(std::string filepath)
{
	std::string contents = FileReader::LoadFileAsString(filepath);
	Document doc;
	doc.Parse(contents.c_str());

	assert(doc.IsObject());

	// create material
	assert(doc["name"].IsString());
	assert(doc["shader"].IsString());
	std::string name = doc["name"].GetString();
	std::string shader = doc["shader"].GetString();

	Material* newMaterial = new Material(ShaderLoader::GetShader(shader), name);
	
	// get textures
	const Value& tex = doc["textures"];
	assert(tex.IsArray());
	for (SizeType i = 0; i < tex.Size(); i++)
	{
		std::string texName = tex[i].GetString();
		newMaterial->AddTexture(TextureLoader::GetTexture(texName));
	}

	// get properties
	const Value& prop = doc["properties"];
	assert(prop.IsArray());
	for (SizeType i = 0; i < prop.Size(); i++)
	{
		assert(prop[i].IsObject());
		const auto object = prop[i].GetObject();
		std::string propName = object["name"].GetString();
		const Value& val = object["value"];

		// deal with vectors
		if (val.IsArray())
		{
			int size = val.Size();

			switch (size)
			{
			case 2:
				newMaterial->AddProperty(propName, glm::vec2(val[0].GetFloat(), val[1].GetFloat()));
				break;
			case 3:
				newMaterial->AddProperty(propName, glm::vec3(val[0].GetFloat(), val[1].GetFloat(), val[2].GetFloat()));
				break;
			case 4:
				newMaterial->AddProperty(propName, glm::vec4(val[0].GetFloat(), val[1].GetFloat(), val[2].GetFloat(), val[3].GetFloat()));
				break;
			}
		}
		// deal with single numbers
		if (val.IsFloat())
		{
			newMaterial->AddProperty<float>(propName, object["value"].GetFloat());
			continue;
		}
		else if (val.IsBool())
		{
			newMaterial->AddProperty<bool>(propName, object["value"].GetBool());
			continue;
		}
		else if (val.IsInt())
		{
			newMaterial->AddProperty<int>(propName, object["value"].GetInt());
			continue;
		}
	}

	return newMaterial;
}

Material* MaterialLoader::GetMaterial(const std::string filename)
{
	return materialLookup[filename];
}
