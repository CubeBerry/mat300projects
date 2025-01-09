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
	if (ImPlot::BeginPlot("Sample Plot", ImVec2(-1, -1)))
	{
		ImPlot::PlotLine("y = f(x)", x_data, y_data, 5);
		ImPlot::EndPlot();
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
