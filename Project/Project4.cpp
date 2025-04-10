#include "Project4.hpp"
#include "Engine.hpp"
#include "imgui.h"
#include "implot.h"

void Project4::Init()
{
}

void Project4::Update(float /*dt*/)
{
}

void Project4::ImGuiDraw(float /*dt*/)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));

	ImGui::Begin("Graph", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	// Degree Control
	ImGui::Text("Control Points:");
	ImGui::SameLine();
	ImGui::Text(std::to_string(controlPoints.size()).c_str());
	ImGui::SameLine();
	if (ImGui::SmallButton("-"))
	{
		if (controlPoints.size() > 1)
		{
			controlPoints.erase(controlPoints.end() - 1);
		}
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+"))
	{
		if (controlPoints.size() < 20)
		{
			controlPoints.emplace_back(0.5, 0.5);
		}
	}
	ImGui::SameLine();
	ImGui::Text("Method:");

	// Draw Graph
	if (ImPlot::BeginPlot("Project4", ImVec2(-1, -1)))
	{
		ImPlot::SetupAxesLimits(0.0, 1.0, 0.0, 1.0, ImGuiCond_Always);

		ImPlot::SetupAxisFormat(ImAxis_X1, "%.1f");
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%.1f");
		ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0.0, 1.0);
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0.0, 1.0);

		// Control Points
		std::vector<double> xs;
		std::vector<double> ys;
		for (int i = 0; i < controlPoints.size(); ++i)
		{
			ImPlot::DragPoint(i, &controlPoints[i].first, &controlPoints[i].second, ImVec4(1.f, 1.f, 1.f, 1.f), 10);

			if (controlPoints[i].first <= 0.0) controlPoints[i].first = 0.0;
			else if (controlPoints[i].first >= 1.0) controlPoints[i].first = 1.0;

			if (controlPoints[i].second <= 0.0) controlPoints[i].second = 0.0;
			else if (controlPoints[i].second >= 1.0) controlPoints[i].second = 1.0;

			xs.push_back(controlPoints[i].first);
			ys.push_back(controlPoints[i].second);
		}
		// Polyline
		ImPlot::PlotLine("Polyline", xs.data(), ys.data(), static_cast<int>(xs.size()));

		// Cubic Spline
		int resolution{ 200 };
		std::vector<std::pair<double, double>> curvePoints;
		for (int r = 0; r < resolution; ++r)
		{
			double t = (static_cast<double>(controlPoints.size()) - 1.0) * static_cast<double>(r) / (resolution - 1);
			curvePoints.push_back(CubicSpline(t));
		}

		std::vector<double> cx, cy;
		for (const auto& p : curvePoints)
		{
			cx.push_back(p.first);
			cy.push_back(p.second);
		}
		ImPlot::PlotLine("Cubic Spline", cx.data(), cy.data(), static_cast<int>(cx.size()));

		ImPlot::EndPlot();
	}

	ImGui::End();
}

void Project4::Restart()
{
	End();
	Init();
}

void Project4::End()
{
}
