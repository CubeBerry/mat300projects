#include "Project1.hpp"
#include "Engine.hpp"
#include "imgui.h"
#include "implot.h"

void Project1::Init()
{
}

void Project1::Update(float /*dt*/)
{
}

void Project1::ImGuiDraw(float /*dt*/)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y - 20));

	ImGui::Begin("Graph", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	// Degree Control
	ImGui::Text("Degree:");
	ImGui::SameLine();
	ImGui::Text("%d", degree);
	ImGui::SameLine();
	if (ImGui::SmallButton("-"))
	{
		if (degree > 1)
		{
			degree--;
			controlPoints.erase(controlPoints.end() - 1);
		}
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+"))
	{
		if (degree < 20)
		{
			degree++;
			controlPoints.push_back(1.0);
		}
	}
	ImGui::SameLine();
	ImGui::Text("Method:");
	ImGui::SameLine();
	if (ImGui::RadioButton("De Casteljau", isDeCasteljau == true)) isDeCasteljau = true;
	ImGui::SameLine();
	if (ImGui::RadioButton("BB-Form", isDeCasteljau == false)) isDeCasteljau = false;

	// Draw Graph
	if (ImPlot::BeginPlot("Project1", ImVec2(ImGui::GetWindowSize().x - 55, -1)))
	{
		ImPlot::SetupAxesLimits(0.0, 1.0, -3.0, 3.0, ImGuiCond_Always);

		ImPlot::SetupAxisFormat(ImAxis_X1, "%.2f");
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%.2f");
		ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0.0, 1.0);
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, -3.0, 3.0);

		for (int i = 0; i <= degree; ++i)
		{
			double tPosition = i / static_cast<double>(degree);
			double temp = tPosition;
			ImPlot::DragPoint(i, &tPosition, &controlPoints[i], ImVec4(1.f, 1.f, 1.f, 1.f), 10);
			tPosition = temp;

			ImDrawList* drawList = ImGui::GetForegroundDrawList();
			ImVec2 cpPosition = ImPlot::PlotToPixels(ImPlotPoint(tPosition, controlPoints[i]));
			std::string t = std::format("{:.2f}", controlPoints[i]);
			drawList->AddText(ImVec2(cpPosition.x + 18.f, cpPosition.y - 18.f), IM_COL32(255, 255, 255, 255), t.c_str());

			if (controlPoints[i] <= -3.0) controlPoints[i] = -3.0;
			else if (controlPoints[i] >= 3.0) controlPoints[i] = 3.0;
		}

		std::vector<double> tValues, pValues;
		int resolution{ 200 };
		tValues.reserve(resolution);
		pValues.reserve(resolution);
		for (int n = 0; n < resolution; ++n)
		{
			double t = static_cast<double>(n) / (resolution - 1);
			double val = isDeCasteljau ? DeCasteljau(controlPoints, t) : BBForm(controlPoints, t);

			tValues.push_back(t);
			pValues.push_back(val);
		}

		ImPlot::PlotLine("p(t)", tValues.data(), pValues.data(), resolution);

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
