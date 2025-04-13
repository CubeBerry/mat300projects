#pragma once
#include <vector>
#include <glm.hpp>
#include <ext/scalar_constants.hpp>

#include "GameState.hpp"

using Point = std::pair<double, double>;

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
	std::vector<Point> controlPoints;
	bool isParabola{ false };
	// Best Fit Line
	Point M;
	double bestTheta;
	// Best Fit Parabola
	Point Q0, Q1, Q2;

	void BestFitLine()
	{
		if (controlPoints.size() < 2) return;

		// Calculate the slope (M)
		M = { 0.0, 0.0 };
		for (const auto& P : controlPoints)
		{
			M.first += P.first;
			M.second += P.second;
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
			for (const auto& P : controlPoints)
			{
				const double dx = P.first - M.first;
				const double dy = P.second - M.second;
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

		// Compute BestFitLine First
		Point n = { -glm::sin(bestTheta), glm::cos(bestTheta) };

		std::vector<Point> S1, S2;
		for (const auto& P : controlPoints)
		{
			double dx = P.first - M.first;
			double dy = P.second - M.second;
			double d = dx * n.first + dy * n.second;
			if (d > 0) S1.push_back(P);
			else if (d < 0) S2.push_back(P);
		}

		Point M1{ 0.0, 0.0 }, M2{ 0.0, 0.0 };
		for (const auto& P : S1)
		{
			M1.first += P.first;
			M1.second += P.second;
		}
		if (!S1.empty())
		{
			M1.first /= static_cast<double>(S1.size());
			M1.second /= static_cast<double>(S1.size());
		}

		for (const auto& P : S2)
		{
			M2.first += P.first;
			M2.second += P.second;
		}
		if (!S2.empty())
		{
			M2.first /= static_cast<double>(S2.size());
			M2.second /= static_cast<double>(S2.size());
		}

		Q0 = M1;
		Q2 = M2;

		double dx1 = M1.first - M.first;
		double dy1 = M1.second - M.second;
		double d1 = glm::sqrt(dx1 * dx1 + dy1 * dy1);

		double dx2 = M2.first - M.first;
		double dy2 = M2.second - M.second;
		double d2 = glm::sqrt(dx2 * dx2 + dy2 * dy2);

		double c = glm::max(d1, d2);

		constexpr int k = 100;
		double best{ std::numeric_limits<double>::max() };
		for (int i = 0; i < k; ++i)
		{
			// From -4c to 4c
			double u = -4.0 * c + (8.0 * c * i) / static_cast<double>(k - 1);
			Point candidate = { M.first + u * n.first, M.second + u * n.second };

			double sum{ 0.0 };
			constexpr int sampleCount{ 50 };
			for (const auto& P : controlPoints)
			{
				double minDistance = std::numeric_limits<double>::max();
				for (int j = 0; j < sampleCount; ++j)
				{
					double t = static_cast<double>(j) / static_cast<double>(sampleCount - 1);
					// Quadratic Bezier Curve
					Point C = { (1.0 - t) * (1.0 - t) * Q0.first + 2.0 * (1.0 - t) * t * candidate.first + t * t * Q2.first,
						(1.0 - t) * (1.0 - t) * Q0.second + 2.0 * (1.0 - t) * t * candidate.second + t * t * Q2.second };
					const double dx = P.first - C.first;
					const double dy = P.second - C.second;
					const double distance = glm::sqrt(dx * dx + dy * dy);
					if (distance < minDistance) minDistance = distance;
				}
				sum += minDistance;
			}

			if (sum < best)
			{
				best = sum;
				Q1 = candidate;
			}
		}
	}
};