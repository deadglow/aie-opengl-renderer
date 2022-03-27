#include "RendererDebugMenu.h"
#include "Renderer.h"
#include <string>
#include <sstream>
#include "imgui.h"

void RendererDebugMenu::DrawImGui()
{
	// render data
	ImGui::Begin("Render Data");
	std::stringstream stream;
	stream.precision(2);

	stream << std::fixed << (1.0f / Renderer::GetDeltaTime());

	ImGui::Text(stream.str().c_str());

	ImGui::InputFloat("Fog density", &Renderer::fogDensity, 0.0001f, 0.01f, "%.5f");
	float col[3] = {Renderer::fogColor.x, Renderer::fogColor.y, Renderer::fogColor.z};
	ImGui::ColorEdit3("Fog density", col);
	Renderer::fogColor = { col[0], col[1], col[2], 1.0f};

	ImGui::End();

	// model creation
	ImGui::Begin("Model");
	char* input = new char[32];
	ImGui::InputText("Model Name", input, 32);
	std::string inputStr;
	ImGui::Button("Create instance");
}
