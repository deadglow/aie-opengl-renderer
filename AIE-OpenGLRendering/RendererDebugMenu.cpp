#include "RendererDebugMenu.h"
#include "Renderer.h"
#include <string>
#include <sstream>
#include "imgui.h"

ModelInstance* RendererDebugMenu::selectedInstance = nullptr;
Model* RendererDebugMenu::selectedBaseModel = nullptr;
Material* RendererDebugMenu::selectedMaterial = nullptr;

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
	ImGui::ColorEdit3("Fog density", col);
	Renderer::fogColor = { col[0], col[1], col[2], 1.0f };

	ImGui::End();
}

void RendererDebugMenu::DrawModelCreation()
{
	ImGui::Begin("Model Creation");
	if (selectedBaseModel)
	{
		if (ImGui::Button("Create instance"))
		{
			ModelInstance* instance = new ModelInstance(selectedBaseModel);
			instance->transform.SetPosition(Renderer::camera.transform.GetPosition());
			Renderer::AddModelInstance(instance);
		}
	}
	else
		ImGui::Button("No model selected.");

	for (const auto entry : ModelLoader::modelList)
	{
		if (!entry.second->IsLoaded()) continue;

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
		ImGui::DragFloat3("Position", posArray);
		pos = { posArray[0], posArray[1], posArray[2] };
		selectedInstance->transform.SetPosition(pos);

		if (ImGui::Button("Delete instance"))
		{
			Renderer::RemoveModelInstance(selectedInstance);
			selectedInstance = nullptr;
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

	for (const auto instance : Renderer::modelInstances)
	{
		bool isSelected = selectedInstance == instance;
		std::string name = instance->GetBaseModel()->GetFilename();
		ImGui::Selectable(name.c_str(), &isSelected);
		if (isSelected)
			selectedInstance = instance;
	}

	ImGui::End();
}

void RendererDebugMenu::DrawMaterialList()
{
	ImGui::Begin("Material List");

	if (selectedMaterial)
	{
		selectedMaterial->UseShader();
		ImGui::Begin("Current material");

		for (int i = 0; i < selectedMaterial->usedTextures.size(); ++i)
		{
			ImGui::Button(selectedMaterial->usedTextures[i]->GetFilename().c_str());
		}

		ImGui::End();
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
