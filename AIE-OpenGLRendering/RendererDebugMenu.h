#pragma once
#include "ModelInstance.h"

class RendererDebugMenu
{
private:
	static ModelInstance* selectedInstance;
	static Model* selectedBaseModel;
	static Material* selectedMaterial;

	static void DrawRenderData();
	static void DrawModelCreation();
	static void DrawInstanceEditing();
	static void DrawInstanceSelection();
	static void DrawMaterialList();
public:
	RendererDebugMenu() = delete;
	RendererDebugMenu(const RendererDebugMenu&) = delete;

	static void DrawImGui();
};
