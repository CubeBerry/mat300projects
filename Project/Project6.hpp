#pragma once
#include <vector>
#include <cmath>

#include "GameState.hpp"

class Project6 : public GameState
{
public:
	Project6() = default;
	~Project6() override {}

	void Init() override;
	void Update(float dt) override;
	void ImGuiDraw(float dt) override;
	void Restart() override;
	void End() override;

private:
	std::vector<std::pair<double, double>> controlPoints = {
		{ 0.5, 0.5 }
	};

	static int BinomialCoefficient(int n, int k)
	{
		int result{ 1 };
		for (int i = 0; i < k; ++i)
		{
			result *= (n - i);
			result /= (i + 1);
		}
		return result;
	}

	std::pair<double, double> BBForm(double t)
	{
		std::pair<double, double> p{ 0.0, 0.0 };
		int n = static_cast<int>(controlPoints.size()) - 1;
		for (int i = 0; i <= n; ++i)
		{
			double coefficient = BinomialCoefficient(n, i);
			double bernstein = coefficient * std::pow(1.0 - t, n - i) * std::pow(t, i);
			p.first += controlPoints[i].first * bernstein;
			p.second += controlPoints[i].second * bernstein;
		}

		return p;
	}
};