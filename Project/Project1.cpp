#include "Project1.hpp"
#include "Engine.hpp"
#include "imgui.h"
#include "implot.h"

void Project1::Init()
{
}

void Project1::Update(float dt)
{
}

void Project1::ImGuiDraw(float /*dt*/)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y - 20));

	ImGui::Begin("Graph", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	// Degree Control
	ImGui::Text("Degree: ");
	ImGui::SameLine();
	ImGui::Text("%d", degree);
	ImGui::SameLine();
	if (ImGui::SmallButton("-"))
	{
		if (degree > 0) degree--;
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+"))
	{
		if (degree < 20) degree++;
	}

	// Draw Graph
	if (ImPlot::BeginPlot("Project1", ImVec2(-1, -1)))
	{
		
	}

	ImGui::End();
}

void Project1::Restart()
{
	End();
	Init();
}

void Project1::End()
{
}
