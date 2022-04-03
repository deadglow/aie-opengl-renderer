#pragma once
#include "ModelInstance.h"

class RendererDebugMenu
{
private:
	static ModelInstance* selectedInstance;
	static Model* selectedBaseModel;
	static Material* selectedMaterial;
	static Texture* selectedTexture;
	static int dropDownSelected;

	static void DrawRenderData();
	static void DrawModelCreation();
	static void DrawInstanceEditing();
	static void DrawInstanceSelection();
	static void DrawMaterialList();
	static void DrawTexturesList();
	static void SelectInstance(ModelInstance* instance);
public:
	RendererDebugMenu() = delete;
	RendererDebugMenu(const RendererDebugMenu&) = delete;

	static void DrawImGui();
};

