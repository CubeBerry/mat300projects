#pragma once
#include <vector>
#include <cmath>

#include "GameState.hpp"

class Project3 : public GameState
{
public:
	Project3() = default;
	~Project3() override {}

	void Init() override;
	void Update(float dt) override;
	void ImGuiDraw(float dt) override;
	void Restart() override;
	void End() override;

private:
	std::vector<std::pair<double, double>> controlPoints = {
		{ 0.5, 0.5 }
	};

	std::pair<double, double> Newton(const double& t) const
	{
		int n = static_cast<int>(controlPoints.size());

		std::vector<double> ti(n);
		for (int i = 0; i < n; ++i)
		{
			ti[i] = i;
		}

		std::vector<double> gx(n), gy(n);
		for (int i = 0; i < n; ++i)
		{
			gx[i] = controlPoints[i].first;
			gy[i] = controlPoints[i].second;
		}

		for (int k = 1; k < n; ++k)
		{
			for (int i = 0; i < n - k; ++i)
			{
				gx[i] = (gx[i + 1] - gx[i]) / (ti[i + k] - ti[i]);
				gy[i] = (gy[i + 1] - gy[i]) / (ti[i + k] - ti[i]);
			}
		}

		std::pair<double, double> result;
		result.first = gx[0];
		result.second = gy[0];
		double product = 1.0;
		for (int i = 1; i < n; ++i)
		{
			product *= t - (i - 1);
			result.first += gx[i] * product;
			result.second += gy[i] * product;
		}
		return result;
	}
};