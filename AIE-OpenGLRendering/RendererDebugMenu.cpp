#include "RendererDebugMenu.h"
#include "Renderer.h"
#include <string>
#include <sstream>
#include "imgui.h"
#include <unordered_map>

ModelInstance* RendererDebugMenu::selectedInstance = nullptr;
Model* RendererDebugMenu::selectedBaseModel = nullptr;
Material* RendererDebugMenu::selectedMaterial = nullptr;
int RendererDebugMenu::dropDownSelected = -1;


void RendererDebugMenu::DrawImGui()
{
	DrawRenderData();

	DrawModelCreation();

	DrawInstanceEditing();

	DrawInstanceSelection();

	DrawMaterialList();
}

void RendererDebugMenu::DrawRenderData()
{
	ImGui::Begin("Render Data");
	std::stringstream stream;
	stream.precision(2);

	stream << std::fixed << (1.0f / Renderer::GetDeltaTime());

	ImGui::Text(stream.str().c_str());

	ImGui::InputFloat("Fog density", &Renderer::fogDensity, 0.0001f, 0.01f, "%.5f");
	float col[3] = { Renderer::fogColor.x, Renderer::fogColor.y, Renderer::fogColor.z };
	ImGui::ColorEdit3("Fog color", col);
	Renderer::fogColor = { col[0], col[1], col[2], 1.0f };

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
		std::string name = entry.second->GetFilename() + " " + std::to_string(entry.second->meshes.size());
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
		glm::vec3 pos = selectedInstance->transform.GetPosition();
		float posArray[3] = { pos.x, pos.y, pos.z };
		ImGui::DragFloat3("Position", posArray, 0.1f);
		pos = { posArray[0], posArray[1], posArray[2] };
		selectedInstance->transform.SetPosition(pos);

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
					for (const auto [key, value] : Renderer::materials)
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

					ImGui::ListBox("Select material", &selected, &(charArrays[0]), charArrays.size());

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
		selectedMaterial->UseShader();

		ImGui::Text("Textures");
		for (int i = 0; i < selectedMaterial->usedTextures.size(); ++i)
		{
			ImGui::Selectable(selectedMaterial->usedTextures[i]->GetFilename().c_str());
		}

		ImGui::Spacing();
		ImGui::Text("Properties");
		for (int i = 0; i < selectedMaterial->properties.size(); ++i)
		{
			ImGui::Selectable(selectedMaterial->properties[i]->name.c_str());
		}

	}
	else
	{
		ImGui::Text("No material selected.");
	}

	for (const auto [key, value] : Renderer::materials)
	{
		bool isSelected = value == selectedMaterial;
		ImGui::Selectable(value->GetName().c_str(), &isSelected);
		if (isSelected)
			selectedMaterial = value;
	}

	ImGui::End();
}

void RendererDebugMenu::SelectInstance(ModelInstance* instance)
{
	selectedInstance = instance;
	dropDownSelected = -1;
}
