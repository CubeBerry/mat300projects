#include "Project6.hpp"
#include "Engine.hpp"
#include "imgui.h"
#include "implot.h"

void Project6::Init()
{
	UpdateKnowSequence();
	UpdateDeBoorControlPoints();
}

void Project6::Update(float /*dt*/)
{
	UpdateKnowSequence();
	UpdateDeBoorControlPoints();
}

void Project6::ImGuiDraw(float /*dt*/)
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

	// Knot Sequence
	if (ImGui::Button("Change Knot Sequence"))
	{
		isNewDefaultKnotSequence = !isNewDefaultKnotSequence;
		UpdateKnowSequence();
		UpdateDeBoorControlPoints();
	}

	std::ostringstream oss;
	for (size_t i = 0; i < knotSequence.size(); ++i)
	{
		oss << knotSequence[i];
		if (i != knotSequence.size() - 1) oss << ", ";
	}
	ImGui::TextWrapped("Knot Sequence: %s", oss.str().c_str());

	// Draw Graph
	if (ImPlot::BeginPlot("Project6", ImVec2(-1, -1)))
	{
		ImPlot::SetupAxesLimits(0.0, 1.0, 0.0, 1.0, ImGuiCond_Always);

		ImPlot::SetupAxisFormat(ImAxis_X1, "%.1f");
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%.1f");
		ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0.0, 1.0);
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0.0, 1.0);

		// Bezier Control Points
		std::vector<double> bcxs;
		std::vector<double> bcys;
		for (int i = 0; i < static_cast<int>(controlPoints.size()); ++i)
		{
			ImPlot::DragPoint(i, &controlPoints[i].first, &controlPoints[i].second, ImVec4(1.f, 1.f, 1.f, 1.f), 10);

			if (controlPoints[i].first <= 0.0) controlPoints[i].first = 0.0;
			else if (controlPoints[i].first >= 1.0) controlPoints[i].first = 1.0;

			if (controlPoints[i].second <= 0.0) controlPoints[i].second = 0.0;
			else if (controlPoints[i].second >= 1.0) controlPoints[i].second = 1.0;

			bcxs.push_back(controlPoints[i].first);
			bcys.push_back(controlPoints[i].second);
		}
		// Bezier Polyline
		ImPlot::PlotLine("Bezier Polyline", bcxs.data(), bcys.data(), static_cast<int>(bcxs.size()));

		// Bezier Curve
		int resolution{ 200 };
		std::vector<std::pair<double, double>> curvePoints;
		std::vector<double> bx, by;
		for (int n = 0; n < resolution; ++n)
		{
			double t = static_cast<double>(n) / (resolution - 1);
			bx.push_back(BBForm(t).first);
			by.push_back(BBForm(t).second);
		}
		ImPlot::PlotLine("Bezier Curve", bx.data(), by.data(), static_cast<int>(bx.size()));

		// De Boor Control Points
		std::vector<double> dcxs, dcys;
		for (int i = 0; i < static_cast<int>(deBoorControlPoints.size()); ++i)
		{
			//ImPlot::DragPoint(i, &deBoorControlPoints[i].first, &deBoorControlPoints[i].second, ImVec4(0.f, 0.f, 1.f, 1.f), 10);

			//if (deBoorControlPoints[i].first <= 0.0) deBoorControlPoints[i].first = 0.0;
			//else if (deBoorControlPoints[i].first >= 1.0) deBoorControlPoints[i].first = 1.0;

			//if (deBoorControlPoints[i].second <= 0.0) deBoorControlPoints[i].second = 0.0;
			//else if (deBoorControlPoints[i].second >= 1.0) deBoorControlPoints[i].second = 1.0;

			dcxs.push_back(deBoorControlPoints[i].first);
			dcys.push_back(deBoorControlPoints[i].second);
		}
		// De Boor Control Points
		ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 10.0f);
		ImPlot::PlotScatter("De Boor Control Points", dcxs.data(), dcys.data(), static_cast<int>(dcxs.size()));
		ImPlot::PopStyleVar();
		// De Boor Polyline
		ImPlot::PlotLine("De Boor Polyline", dcxs.data(), dcys.data(), static_cast<int>(dcxs.size()));

		// De Boor Curve
		std::vector<double> dx, dy;
		for (int n = 0; n < resolution; ++n)
		{
			double t = static_cast<double>(n) / (resolution - 1);
			dx.push_back(DeBoor(t).first);
			dy.push_back(DeBoor(t).second);
		}
		ImPlot::PlotLine("De Boor Curve", dx.data(), dy.data(), static_cast<int>(dx.size()));

		ImPlot::EndPlot();
	}

	ImGui::End();
}

void Project6::Restart()
{
	End();
	Init();
}

void Project6::End()
{
}
