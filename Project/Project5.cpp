#include "Project5.hpp"
#include "Engine.hpp"
#include "imgui.h"
#include "implot.h"

void Project5::Init()
{
	degree = 3;
	N = degree + 10;
	controlPoints.resize(N - degree, 1.0);
	method = Method::DB;
}

void Project5::Update(float /*dt*/)
{
}

void Project5::ImGuiDraw(float /*dt*/)
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
	if (ImGui::SmallButton("-D"))
	{
		if (degree > 1)
		{
			degree--;
			N = std::max(degree + 10, 2 * degree);
			controlPoints.resize(N - degree, 1.0);
		}
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+D"))
	{
		if (degree < 9)
		{
			degree++;
			N = degree + 10;
			controlPoints.resize(N - degree, 1.0);
		}
	}
	ImGui::SameLine();
	// N Control
	ImGui::Text("N:");
	ImGui::SameLine();
	ImGui::Text("%d", N);
	ImGui::SameLine();
	if (ImGui::SmallButton("-N"))
	{
		if (N > degree + 10) N--;
		if (N < 2 * degree) N = 2 * degree;
		controlPoints.resize(N - degree, 1.0);
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+N"))
	{
		if (N < 40) N++;
		controlPoints.resize(N - degree, 1.0);
	}
	ImGui::SameLine();
	ImGui::Text("Method:");
	ImGui::SameLine();
	if (ImGui::RadioButton("De Boor", method == Method::DB)) method = Method::DB;
	ImGui::SameLine();
	if (ImGui::RadioButton("Divided Differences", method == Method::DD)) method = Method::DD;
	ImGui::SameLine();
	if (ImGui::RadioButton("Sum of Shifted Power Functions", method == Method::SSPF)) method = Method::SSPF;

	// Draw Graph
	if (ImPlot::BeginPlot("Project5", ImVec2(ImGui::GetWindowSize().x - 55, -1)))
	{
		double x_min = (degree + 1) / 2.0;
		double x_max = x_min + static_cast<double>(controlPoints.size()) - 1.0;
		ImPlot::SetupAxesLimits(x_min, x_max, -3.0, 3.0, ImGuiCond_Always);

		ImPlot::SetupAxisFormat(ImAxis_X1, "%.1f");
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%.1f");
		ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, x_min, x_max);
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, -3.0, 3.0);

		// Control Points
		// i < controlPoints.size() == N - degree - 1
		for (int i = 0; i < static_cast<int>(controlPoints.size()); ++i)
		{
			double cpX = (degree + 1) / 2.0 + i;
			double cpY = controlPoints[i];

			double temp = cpX;
			ImPlot::DragPoint(i, &cpX, &cpY, ImVec4(1.f, 1.f, 1.f, 1.f), 10);
			cpX = temp;
			controlPoints[i] = cpY;

			ImDrawList* drawList = ImGui::GetForegroundDrawList();
			ImVec2 cpPosition = ImPlot::PlotToPixels(ImPlotPoint(cpX, cpY));
			std::string t = std::format("{:.2f}", cpY);
			drawList->AddText(ImVec2(cpPosition.x + 18.f, cpPosition.y - 18.f), IM_COL32(255, 255, 255, 255), t.c_str());

			if (controlPoints[i] <= -3.0) controlPoints[i] = -3.0;
			else if (controlPoints[i] >= 3.0) controlPoints[i] = 3.0;
		}

		double t_min = degree;
		double t_max = N - degree;
		int resolution = 200;
		std::vector<double> tValues, splineValues;
		tValues.reserve(resolution);
		splineValues.reserve(resolution);
		for (int n = 0; n < resolution; ++n)
		{
			double t = t_min + static_cast<double>(n) / (resolution - 1) * (t_max - t_min);
			tValues.push_back(t);
			switch (method)
			{
			case Method::DB:
			{
				splineValues.emplace_back(DeBoor(t));
			}
			break;
			case Method::DD:
				splineValues.emplace_back(DividedDifferences(t));
				break;
			case Method::SSPF:
				splineValues.emplace_back(SumOfShiftedPowerFunctions(t));
				break;
			}
		}
		ImPlot::PlotLine("f(t)", tValues.data(), splineValues.data(), resolution);

		ImPlot::EndPlot();
	}

	ImGui::End();
}

void Project5::Restart()
{
	End();
	Init();
}

void Project5::End()
{
}
