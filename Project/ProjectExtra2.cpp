#include "ProjectExtra2.hpp"
#include "Engine.hpp"
#include "imgui.h"
#include "implot.h"

void ProjectExtra2::Init()
{
	controlPoints.clear();
	controlPoints.push_back({ 0.2, 0.7 });
	controlPoints.push_back({ 0.5, 0.3 });
	controlPoints.push_back({ 0.8, 0.8 });
}

void ProjectExtra2::Update(float /*dt*/)
{
	BestFitLine();
	if (isParabola) BestFitParabola();
}

void ProjectExtra2::ImGuiDraw(float /*dt*/)
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
		if (controlPoints.size() > 2)
		{
			controlPoints.erase(controlPoints.end() - 1);
		}
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+"))
	{
		controlPoints.push_back({ 0.5, 0.5 });
	}
	ImGui::SameLine();
	// Toggle Parabola
	if (ImGui::Checkbox("Show Parabola", &isParabola))
	{
		if (isParabola)
		{
			BestFitLine();
			BestFitParabola();
		}
	}

	// Draw Graph
	if (ImPlot::BeginPlot("ProjectExtra2", ImVec2(-1, -1)))
	{
		ImPlot::SetupAxesLimits(0.0, 1.0, 0.0, 1.0, ImGuiCond_Always);

		ImPlot::SetupAxisFormat(ImAxis_X1, "%.1f");
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%.1f");
		ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0.0, 1.0);
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0.0, 1.0);

		// Control Points
		for (int i = 0; i < static_cast<int>(controlPoints.size()); ++i)
		{
			ImPlot::DragPoint(i, &controlPoints[i].first, &controlPoints[i].second, ImVec4(1.f, 1.f, 1.f, 1.f), 10);

			if (controlPoints[i].first <= 0.0) controlPoints[i].first = 0.0;
			else if (controlPoints[i].first >= 1.0) controlPoints[i].first = 1.0;

			if (controlPoints[i].second <= 0.0) controlPoints[i].second = 0.0;
			else if (controlPoints[i].second >= 1.0) controlPoints[i].second = 1.0;
		}

		// Best Fit Line
		if (!isParabola)
		{
			double length = 1.0;
			Point p0 = { M.first - length * glm::cos(bestTheta), M.second - length * glm::sin(bestTheta) };
			Point p1 = { M.first + length * glm::cos(bestTheta), M.second + length * glm::sin(bestTheta) };
			double lx[2] = { p0.first, p1.first };
			double ly[2] = { p0.second, p1.second };
			ImPlot::PlotLine("Best Fit Line", lx, ly, 2);
		}
		// Best Fit Parabola
		else
		{
			int resolution{ 200 };
			std::vector<double> px, py;
			for (int n = 0; n < resolution; ++n)
			{
				double t = static_cast<double>(n) / (resolution - 1);
				double u = 1 - t;
				Point pt = { u * u * Q0.first + 2 * u * t * Q1.first + t * t * Q2.first,
								u * u * Q0.second + 2 * u * t * Q1.second + t * t * Q2.second };
				px.push_back(pt.first);
				py.push_back(pt.second);
			}
			ImPlot::PlotLine("Best Fit Parabola", px.data(), py.data(), static_cast<int>(px.size()));
		}

		ImPlot::EndPlot();
	}

	ImGui::End();
}

void ProjectExtra2::Restart()
{
	End();
	Init();
}

void ProjectExtra2::End()
{
}
