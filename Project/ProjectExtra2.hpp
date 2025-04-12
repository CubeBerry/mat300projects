#pragma once
#include <vector>
#include <glm.hpp>
#include <ext/scalar_constants.hpp>

#include "GameState.hpp"

class ProjectExtra2 : public GameState
{
public:
	ProjectExtra2() = default;
	~ProjectExtra2() override {}

	void Init() override;
	void Update(float dt) override;
	void ImGuiDraw(float dt) override;
	void Restart() override;
	void End() override;
private:
	std::vector<std::pair<double, double>> controlPoints;
	bool isParabola{ false };
	std::pair<double, double> M;
	double bestTheta;

	void BestFitLine()
	{
		if (controlPoints.size() < 2) return;

		// Calculate the slope (M)
		M = { 0.0, 0.0 };
		for (const auto& cp : controlPoints)
		{
			M.first += cp.first;
			M.second += cp.second;
		}
		M.first /= static_cast<double>(controlPoints.size());
		M.second /= static_cast<double>(controlPoints.size());

		constexpr int k = 100;
		double best{ std::numeric_limits<double>::max() };
		for (int i = 0; i < k; ++i)
		{
			// From 0 to pi
			double theta = glm::pi<double>() * i / static_cast<double>(k - 1);
			double sum{ 0.0 };
			for (const auto& cp : controlPoints)
			{
				const double dx = cp.first - M.first;
				const double dy = cp.second - M.second;
				// (-sin, cos) is the normal vector
				const double distance = glm::abs(-glm::sin(theta) * dx + glm::cos(theta) * dy);
				sum += distance;
			}
			// Minimize the sum of distances
			if (sum < best)
			{
				best = sum;
				bestTheta = theta;
			}
		}
	}

	void BestFitParabola()
	{
		if (controlPoints.size() < 3) return;
	}
};