#include "Project2.hpp"
#include "Engine.hpp"
#include "imgui.h"
#include "implot.h"

void Project2::Init()
{
}

void Project2::Update(float /*dt*/)
{
}

void Project2::ImGuiDraw(float /*dt*/)
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
		if (controlPoints.size() > 0)
		{
			controlPoints.erase(controlPoints.end() - 1);
		}
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+"))
	{
		if (controlPoints.size() < 20)
		{
			controlPoints.push_back({ 0.5, 0.5 });
		}
	}
	ImGui::SameLine();
	ImGui::Text("Method:");
	ImGui::SameLine();
	if (ImGui::RadioButton("NLI", method == METHOD::NLI)) method = METHOD::NLI;
	ImGui::SameLine();
	if (ImGui::RadioButton("BB-Form", method == METHOD::BB)) method = METHOD::BB;
	ImGui::SameLine();
	if (ImGui::RadioButton("Midpoint Subdivision", method == METHOD::MIDPOINT)) method = METHOD::MIDPOINT;
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);
	ImGui::SliderFloat("t-value", &shellT, 0.f, 1.f, "%.2f");

	// Draw Graph
	if (ImPlot::BeginPlot("Project2", ImVec2(-1, -1)))
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

		// Bezier Curve
		int resolution{ 200 };
		std::vector<std::pair<double, double>> curvePoints;
		switch (method)
		{
		case METHOD::NLI:
		{
			for (int n = 0; n < resolution; ++n)
			{
				double t = static_cast<double>(n) / (resolution - 1);
				curvePoints.push_back(DeCasteljau(t));
			}

			std::vector<std::vector<std::pair<double, double>>> shells = DeCasteljauShells(static_cast<double>(shellT));
			for (size_t i = 0; i < shells.size(); ++i)
			{
				std::vector<double> sx, sy;
				for (auto& s : shells[i])
				{
					sx.push_back(s.first);
					sy.push_back(s.second);
				}

				ImPlot::PlotLine("Shell", sx.data(), sy.data(), static_cast<int>(sx.size()));
				ImPlot::PlotScatter("Shell", sx.data(), sy.data(), static_cast<int>(sx.size()));
			}
		}
		break;
		case METHOD::BB:
			for (int n = 0; n < resolution; ++n)
			{
				double t = static_cast<double>(n) / (resolution - 1);
				curvePoints.push_back(BBForm(t));
			}
			break;
		case METHOD::MIDPOINT:
			curvePoints = MidpointSubdivision();
			break;
		}

		std::vector<double> cx, cy;
		for (const auto& p : curvePoints)
		{
			cx.push_back(p.first);
			cy.push_back(p.second);
		}
		ImPlot::PlotLine("Bezier Curve", cx.data(), cy.data(), static_cast<int>(cx.size()));

		ImPlot::EndPlot();
	}

	ImGui::End();
}

void Project2::Restart()
{
	End();
	Init();
}

void Project2::End()
{
}
