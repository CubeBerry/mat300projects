#include "ProjectExtra1.hpp"
#include "Engine.hpp"
#include "imgui.h"
#include "implot.h"

void ProjectExtra1::Init()
{
	controlPoints.clear();
	controlPoints.emplace_back(HermitePoint{ {0.5, 0.5},  {1.0, 0.5}, false });
}

void ProjectExtra1::Update(float /*dt*/)
{
}

void ProjectExtra1::ImGuiDraw(float /*dt*/)
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
			controlPoints.emplace_back(HermitePoint{ {0.5, 0.5},  {1.0, 0.5}, false });
		}
	}

	ImGui::Columns(2, "Columns");
	ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.8f);

	// Draw Graph
	if (ImPlot::BeginPlot("ProjectExtra1", ImVec2(-1, -1)))
	{
		ImPlot::SetupAxesLimits(0.0, 1.0, 0.0, 1.0, ImGuiCond_Always);

		ImPlot::SetupAxisFormat(ImAxis_X1, "%.1f");
		ImPlot::SetupAxisFormat(ImAxis_Y1, "%.1f");
		ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0.0, 1.0);
		ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0.0, 1.0);

		std::vector<double> tx, ty;
		for (int i = 0; i < static_cast<int>(controlPoints.size()); ++i)
		{
			// Hermite Control Points
			ImPlot::DragPoint(i, &controlPoints[i].p.first, &controlPoints[i].p.second, ImVec4(1.f, 1.f, 1.f, 1.f), 10);

			if (controlPoints[i].p.first <= 0.0) controlPoints[i].p.first = 0.0;
			else if (controlPoints[i].p.first >= 1.0) controlPoints[i].p.first = 1.0;

			if (controlPoints[i].p.second <= 0.0) controlPoints[i].p.second = 0.0;
			else if (controlPoints[i].p.second >= 1.0) controlPoints[i].p.second = 1.0;

			// tangent vectors
			if (controlPoints[i].isShowTangent)
			{
				ImPlot::DragPoint(i + static_cast<int>(controlPoints.size()), &controlPoints[i].t.first, &controlPoints[i].t.second, ImVec4(0.f, 1.f, 0.f, 1.f), 10);

				if (controlPoints[i].t.first <= 0.0) controlPoints[i].t.first = 0.0;
				else if (controlPoints[i].t.first >= 1.0) controlPoints[i].t.first = 1.0;

				if (controlPoints[i].t.second <= 0.0) controlPoints[i].t.second = 0.0;
				else if (controlPoints[i].t.second >= 1.0) controlPoints[i].t.second = 1.0;
			}
		}

		// Hermite Curve
		int resolution{ 200 };
		std::vector<std::pair<double, double>> curvePoints;
		std::vector<double> hx, hy;
		for (int n = 0; n < resolution; ++n)
		{
			double t = static_cast<double>(n) / (resolution - 1);
			hx.push_back(HermiteInterpolation(t).first);
			hy.push_back(HermiteInterpolation(t).second);
		}
		ImPlot::PlotLine("Hermite Curve", hx.data(), hy.data(), static_cast<int>(hx.size()));

		ImPlot::EndPlot();
	}

	ImGui::NextColumn();

	// Control Points Menu
	ImGui::Text("Control Points Menu");
	ImGui::Separator();
	for (int i = 0; i < static_cast<int>(controlPoints.size()); i++)
	{
		std::ostringstream oss;
		oss << "Control Point " << i << "("
		<< std::fixed << std::setprecision(2)
		<< controlPoints[i].p.first << " "
		<< controlPoints[i].p.second << ")";
		if (ImGui::CollapsingHeader(oss.str().c_str()))
		{
			double min{ 0.0 }, max{ 1.0 };
			ImGui::DragScalar("Tangent X", ImGuiDataType_Double, &controlPoints[i].t.first, 0.001f, &min, &max);
			ImGui::DragScalar("Tangent Y", ImGuiDataType_Double, &controlPoints[i].t.second, 0.001f, &min, &max);
			if (ImGui::MenuItem("Toggle ShowTangent"))
			{
				controlPoints[i].isShowTangent = !controlPoints[i].isShowTangent;
			}
		}
	}
	ImGui::Columns(1);

	ImGui::End();
}

void ProjectExtra1::Restart()
{
	End();
	Init();
}

void ProjectExtra1::End()
{
}
