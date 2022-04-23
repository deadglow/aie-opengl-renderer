#include "RendererDebugMenu.h"
#include "Renderer.h"
#include <string>
#include <sstream>
#include "imgui.h"
#include <unordered_map>
#include <typeinfo>

ModelInstance* RendererDebugMenu::selectedInstance = nullptr;
Model* RendererDebugMenu::selectedBaseModel = nullptr;
Material* RendererDebugMenu::selectedMaterial = nullptr;
Texture* RendererDebugMenu::selectedTexture = nullptr;
Light* RendererDebugMenu::selectedLight = nullptr;
int RendererDebugMenu::dropDownSelected = -1;
glm::vec3 RendererDebugMenu::instanceRotation;
glm::vec3 RendererDebugMenu::lightRotation;


glm::quat RotationControl(glm::vec3* rotation)
{
	glm::vec3 degrees = glm::degrees(*rotation);
	ImGui::DragFloat3("Rotation", (float*)&degrees, 0.5f);
	*rotation = glm::radians(degrees);

	glm::quat quat = glm::identity<glm::quat>();
	return quat * glm::angleAxis(rotation->y, glm::vec3(0, 1, 0)) * glm::angleAxis(rotation->x, glm::vec3(1, 0, 0)) * glm::angleAxis(rotation->z, glm::vec3(0, 0, 1));
}

void RendererDebugMenu::DrawImGui()
{
	DrawRenderData();

	DrawModelCreation();

	DrawInstanceEditing();

	DrawInstanceSelection();

	DrawMaterialList();

	DrawTexturesList();

	DrawLightingList();
}

void RendererDebugMenu::DrawRenderData()
{
	ImGui::Begin("Render Data");
	std::stringstream stream;
	stream.precision(2);

	stream << std::fixed << (1.0f / Renderer::GetDeltaTime());

	ImGui::Text(stream.str().c_str());

	ImGui::InputFloat("Fog density", &Renderer::fogDensity, 0.0001f, 0.01f, "%.5f");
	float* color = (float*)&Renderer::fogColor;
	ImGui::ColorEdit3("Fog color", color);

	ImGui::End();
}

void RendererDebugMenu::DrawModelCreation()
{
	ImGui::Begin("Model Creation");
	if (selectedBaseModel)
	{
		if (selectedBaseModel->IsLoaded())
		{
			if (ImGui::Button("Create instance"))
			{
				ModelInstance* instance = new ModelInstance(selectedBaseModel);
				instance->transform.SetPosition(Renderer::cameraStack.front().transform.GetPosition());
				Renderer::AddModelInstance(instance);
				SelectInstance(instance);
			}
			ImGui::SameLine();
			if (ImGui::Button("Unload"))
			{
				selectedBaseModel->Unload();
			}
		}
		else
		{
			if (ImGui::Button("Load"))
			{
				selectedBaseModel->Load();
			}
		}
	}
	else
		ImGui::Button("No model selected.");

	for (const auto entry : ModelLoader::modelList)
	{
		bool isSelected = selectedBaseModel == entry.second;
		std::string name = entry.second->GetFilename();
		ImGui::Selectable(name.c_str(), &isSelected);
		if (isSelected)
			selectedBaseModel = entry.second;
	}

	ImGui::End();
}

void RendererDebugMenu::DrawInstanceEditing()
{
	ImGui::Begin("Instance Editing");

	if (selectedInstance)
	{
		ImGui::Text((std::string("Current model instance: ") + selectedInstance->GetBaseModel()->GetFilename()).c_str());

		// pos
		glm::vec3 pos = selectedInstance->transform.GetPosition();
		float posArray[3] = { pos.x, pos.y, pos.z };
		ImGui::DragFloat3("Position", posArray, 0.1f);
		pos = { posArray[0], posArray[1], posArray[2] };
		selectedInstance->transform.SetPosition(pos);
		ImGui::SameLine();
		if (ImGui::Button("ResetPos"))
			selectedInstance->transform.SetPosition({ 0, 0, 0 });

		selectedInstance->transform.SetRotation(RotationControl(&instanceRotation));
		
		ImGui::SameLine();
		if (ImGui::Button("ResetRot"))
			instanceRotation = { 0, 0, 0 };


		if (ImGui::Button("Delete instance"))
		{
			Renderer::RemoveModelInstance(selectedInstance);
			SelectInstance(nullptr);
			ImGui::End();
			return;
		}

		ImGui::Spacing();
		ImGui::Text("Material Overrides");
		if (selectedInstance->IsOverridingMaterials())
		{
			for (int i = 0; i < selectedInstance->materialOverrides.size(); ++i)
			{
				if (ImGui::Button((std::to_string(i) + ", " + selectedInstance->materialOverrides[i]->GetName()).c_str()))
				{
					dropDownSelected = i;
				}

				if (i == dropDownSelected)
				{
					// generate list of materials
					std::vector<Material*> matList;
					std::vector<std::string> materialNames;
					int count = 0;
					int selected = -1;
					for (const auto [key, value] : MaterialLoader::materialLookup)
					{
						matList.push_back(value);
						materialNames.push_back(value->GetName());
						
						// store the index of the currently used material
						if (selectedInstance->materialOverrides[i] == value)
							selected = count;

						count++;
					}

					// build char* array
					std::vector<const char*> charArrays;
					for (int j = 0; j < materialNames.size(); ++j)
					{
						charArrays.push_back(materialNames[j].c_str());
					}

					ImGui::ListBox("Select material", &selected, &(charArrays[0]), (int)charArrays.size());

					selectedInstance->SetMaterialOverride(i, matList[selected]);
				}

			}


			if (ImGui::Button("Clear Material Overrides"))
			{
				selectedInstance->ClearMaterialOverrides();
			}
		}
		else
		{
			if (ImGui::Button("Override Materials"))
			{
				selectedInstance->SetMaterialOverride(0, selectedInstance->GetBaseModel()->defaultMaterials[0]);
			}
		}
	}
	else
	{
		ImGui::Text("No instance selected.");
	}

	ImGui::End();
}

void RendererDebugMenu::DrawInstanceSelection()
{
	ImGui::Begin("Instance Selection");

	std::unordered_map<std::string, int> nameList;

	for (const auto instance : Renderer::modelInstances)
	{
		bool isSelected = selectedInstance == instance;
		std::string name = instance->GetBaseModel()->GetFilename();
		
		// count names
		if (nameList.count(name) > 0)
		{
			nameList[name]++;
			name = name + " " + std::to_string(nameList[name] - 1);
		}
		else
		{
			nameList.emplace(name, 1);
		}

		ImGui::Selectable(name.c_str(), &isSelected);
		if (isSelected)
		{
			if (selectedInstance != instance)
				SelectInstance(instance);
		}
	}

	ImGui::End();
}

void RendererDebugMenu::DrawMaterialList()
{
	ImGui::Begin("Material List");

	if (selectedMaterial)
	{
		selectedMaterial->UseMaterial();

		ImGui::Text("Textures");
		for (int i = 0; i < selectedMaterial->usedTextures.size(); ++i)
		{
			ImGui::Selectable(selectedMaterial->usedTextures[i]->GetFilename().c_str());
		}

		ImGui::Spacing();
		ImGui::Text("Properties");
		for (int i = 0; i < selectedMaterial->properties.size(); ++i)
		{
			ShaderPropertyBase* prop = selectedMaterial->properties[i];

			ShaderProperty<glm::vec4>* col = dynamic_cast<ShaderProperty<glm::vec4>*>(prop);
			if (col)
			{
				float* floats = (float*)&col->value;
				ImGui::ColorEdit4(col->name.c_str(), floats);
				continue;
			}
			ShaderProperty<float>* flt = dynamic_cast<ShaderProperty<float>*>(prop);
			if (flt)
			{
				ImGui::DragFloat(flt->name.c_str(), &flt->value, 0.001f);
				continue;
			}

			ImGui::Selectable(prop->name.c_str());
		}

	}
	else
	{
		ImGui::Text("No material selected.");
	}

	for (const auto [key, value] : MaterialLoader::materialLookup)
	{
		if (key == SHADER_DEFAULT_UNLIT) continue;
		bool isSelected = value == selectedMaterial;
		ImGui::Selectable(value->GetName().c_str(), &isSelected);
		if (isSelected)
			selectedMaterial = value;
	}

	ImGui::End();
}

void RendererDebugMenu::DrawTexturesList()
{
	ImGui::Begin("Texture List");

	if (selectedTexture)
	{
		if (selectedTexture->IsLoaded())
		{
			if (ImGui::Button("Unload"))
				selectedTexture->Unload();

			if (selectedTexture->GetTexType() == TEX_Type::Cubemap)
			{
				ImGui::SameLine();
				if (ImGui::Button("Set Skybox"))
				{
					Renderer::SetSkybox((Cubemap*)selectedTexture);
				}
			}
		}
		else
		{
			if (ImGui::Button("Load"))
				selectedTexture->Load();
		}
	}

	for (const auto [key, value] : TextureLoader::textureLookup)
	{
		if (key == TEXTURE_DEFAULT_ERROR) continue;

		bool isSelected = value == selectedTexture;
		ImGui::Selectable(value->GetFilename().c_str(), &isSelected);
		if (isSelected)
			selectedTexture = value;
	}

	ImGui::End();
}

void RendererDebugMenu::DrawLightingList()
{
	ImGui::Begin("Lighting");
	ImGui::ColorEdit3("Ambient light", (float*)&Renderer::ambientLight);
	
	ImGui::Text("Bloom");
	float bloomThreshold = Renderer::GetBloomThreshold();
	ImGui::DragFloat("Threshold", &bloomThreshold, 0.01, 0, 1000);
	Renderer::SetBloomThreshold(bloomThreshold);

	ImGui::InputInt("Samples", &Renderer::bloomBlurSamples, 1, 3);

	float exposure = Renderer::GetExposure();
	ImGui::DragFloat("Exposure", &exposure, 0.01);
	Renderer::SetExposure(exposure);

	ImGui::Spacing();
	// draw selected light
	if (selectedLight)
	{
		glm::mat4 lightMat = selectedLight->transform.matrix;
		lightMat = glm::scale(lightMat, glm::vec3(0.1f, 0.1f, 0.1f));
		ModelLoader::GetModel(MODEL_DEFAULT)->Draw(Renderer::cameraStack.front().GetShaderData(), lightMat);
		ImGui::Text(Light::GetTypeName(selectedLight->GetType()).c_str());
		
		if (ImGui::Button("Delete"))
		{
			Renderer::lights.remove(selectedLight);
			delete selectedLight;
			SelectLight(nullptr);
			ImGui::End();
			return;
		}

		glm::vec3 position = selectedLight->transform.GetPosition();
		ImGui::DragFloat3("Position", (float*)&position, 0.01f);
		selectedLight->transform.SetPosition(position);

		selectedLight->transform.SetRotation(RotationControl(&lightRotation));

		ImGui::ColorEdit3("Color", (float*)&selectedLight->color);
		ImGui::DragFloat("Intensity", &selectedLight->intensity, 0.001f, 0, 100);

		if (selectedLight->GetType() == LightType::Point)
		{
			PointLight* point = (PointLight*)selectedLight;

			ImGui::DragFloat("Constant", &point->constant, 0.001f);
			ImGui::DragFloat("Linear", &point->linear, 0.001f);
			ImGui::DragFloat("Quadratic", &point->quadratic, 0.001f);
			ImGui::DragFloat("Radius", &point->radius, 0.001f);
		}
	}
	else
	{
		ImGui::Text("No Light Selected");
		ImGui::Spacing();
		ImGui::Spacing();
	}

	if (ImGui::Button("Create Directional Light"))
	{
		DirectionalLight* dirLight = new DirectionalLight();
		Renderer::lights.push_back(dirLight);

		SelectLight(dirLight);
	}
	ImGui::SameLine();
	if (ImGui::Button("Create Point Light"))
	{
		PointLight* pointLight = new PointLight();
		Renderer::lights.push_back(pointLight);

		SelectLight(pointLight);
	}

	// draw list
	for (Light* light : Renderer::lights)
	{
		bool isSelected = light == selectedLight;
		ImGui::Selectable(Light::GetTypeName(light->GetType()).c_str(), &isSelected);
		if (isSelected)
			selectedLight = light;
	}

	ImGui::End();
}

void RendererDebugMenu::SelectInstance(ModelInstance* instance)
{
	selectedInstance = instance;
	dropDownSelected = -1;
	if (selectedInstance)
		instanceRotation = glm::eulerAngles(selectedInstance->transform.GetRotation());
}

void RendererDebugMenu::SelectLight(Light* light)
{
	selectedLight = light;
	if (selectedLight)
		lightRotation = glm::eulerAngles(light->transform.GetRotation());
}
