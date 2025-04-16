#include "Project7.hpp"
#include "Engine.hpp"
#include "imgui.h"
#include "implot.h"

void Project7::Init()
{
	UpdateKnotSequence();
}

void Project7::Update(float /*dt*/)
{
}

void Project7::ImGuiDraw(float /*dt*/)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));

	ImGui::Begin("Graph", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	// Control Points Control
	ImGui::Text("Control Points:");
	ImGui::SameLine();
	ImGui::Text(std::to_string(controlPoints.size()).c_str());
	ImGui::SameLine();
	if (ImGui::SmallButton("-CP"))
	{
		if (controlPoints.size() > static_cast<size_t>(degree) + 1)
		{
			controlPoints.erase(controlPoints.end() - 1);
			UpdateKnotSequence();
		}
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+CP"))
	{
		if (controlPoints.size() < 20)
		{
			controlPoints.emplace_back(0.5, 0.5);
			UpdateKnotSequence();
		}
	}
	ImGui::SameLine();
	// Degree
	ImGui::Text("Degree:");
	ImGui::SameLine();
	ImGui::Text(std::to_string(degree).c_str());
	ImGui::SameLine();
	if (ImGui::SmallButton("-D"))
	{
		if (degree > 1)
		{
			degree--;
			UpdateKnotSequence();
		}
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+D"))
	{
		if (degree < static_cast<int>(controlPoints.size()) - 1)
		{
			degree++;
			UpdateKnotSequence();
		}
	}
	ImGui::SameLine();
	// shell t value
	ImGui::SetNextItemWidth(200.f);
	ImGui::SliderFloat("t-value", reinterpret_cast<float*>(&shellT), static_cast<float>(degree), static_cast<float>(controlPoints.size()), "%.2f");
	// Knot Sequence
	std::ostringstream oss;
	for (size_t i = 0; i < knotSequence.size(); ++i)
	{
		oss << knotSequence[i];
		if (i != knotSequence.size() - 1) oss << ", ";
	}
	ImGui::TextWrapped("Knot Sequence: %s", oss.str().c_str());

	// Draw Graph
	if (ImPlot::BeginPlot("Project7", ImVec2(-1, -1)))
	{
		ImPlot::SetupAxesLimits(0.0, 1.0, 0.0, 1.0, ImGuiCond_Always);

		ImPlot::SetupAxisFormat(ImAxis_X1, "%.1f");
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%.1f");
		ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0.0, 1.0);
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0.0, 1.0);

		// Control Points
		std::vector<double> cx;
		std::vector<double> cy;
		for (int i = 0; i < controlPoints.size(); ++i)
		{
			ImPlot::DragPoint(i, &controlPoints[i].first, &controlPoints[i].second, ImVec4(1.f, 1.f, 1.f, 1.f), 10);

			if (controlPoints[i].first <= 0.0) controlPoints[i].first = 0.0;
			else if (controlPoints[i].first >= 1.0) controlPoints[i].first = 1.0;

			if (controlPoints[i].second <= 0.0) controlPoints[i].second = 0.0;
			else if (controlPoints[i].second >= 1.0) controlPoints[i].second = 1.0;

			cx.push_back(controlPoints[i].first);
			cy.push_back(controlPoints[i].second);
		}
		// Polyline
		ImPlot::PlotLine("Control Polyline", cx.data(), cy.data(), static_cast<int>(cx.size()));

		int resolution{ 200 };
		std::vector<double> dcxs, dcys;
		int s = static_cast<int>(controlPoints.size()) - 1;
		int N = s + degree + 1;
		double tMin{ knotSequence[degree] };
		double tMax{ knotSequence[N - degree] };
		for (int n = 0; n < resolution; ++n)
		{
			double t = tMin + (tMax - tMin) * n / (resolution - 1);
			std::pair<double, double> value = DeBoor(t);
			dcxs.push_back(value.first);
			dcys.push_back(value.second);
		}
		// B-Spline Curve
		ImPlot::PlotLine("B-Spline Curve", dcxs.data(), dcys.data(), static_cast<int>(dcxs.size()));

		// De Boor Shells
		std::vector<std::vector<std::pair<double, double>>> shells = DeBoorShells(static_cast<double>(shellT));
		for (size_t i = 0; i < shells.size(); ++i)
		{
			std::vector<double> sx, sy;
			for (auto& shell : shells[i])
			{
				sx.push_back(shell.first);
				sy.push_back(shell.second);
			}

			ImPlot::PlotLine("Shell", sx.data(), sy.data(), static_cast<int>(sx.size()));
			ImPlot::PlotScatter("Shell", sx.data(), sy.data(), static_cast<int>(sx.size()));
		}

		ImPlot::EndPlot();
	}

	ImGui::End();
}

void Project7::Restart()
{
	End();
	Init();
}

void Project7::End()
{
}
