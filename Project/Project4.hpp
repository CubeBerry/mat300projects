#pragma once
#include <vector>

#include "GameState.hpp"

class Project4 : public GameState
{
public:
	Project4() = default;
	~Project4() override {}

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

		std::vector<std::vector<double>> gx(n, std::vector<double>(n)), gy(n, std::vector<double>(n));
		for (int i = 0; i < n; ++i)
		{
			gx[i][0] = controlPoints[i].first;
			gy[i][0] = controlPoints[i].second;
		}

		for (int i = 1; i < n; ++i)
		{
			for (int j = 0; j < n - i; ++j)
			{
				gx[j][i] = (gx[j][i - 1] - gx[j + 1][i - 1]) / (ti[j] - ti[i + j]);
				gy[j][i] = (gy[j][i - 1] - gy[j + 1][i - 1]) / (ti[j] - ti[i + j]);
			}
		}

		std::pair<double, double> result;
		result.first = gx[0][0];
		result.second = gy[0][0];
		double product = 1.0;
		for (int i = 1; i < n; ++i)
		{
			product *= t - ti[i - 1];
			result.first += gx[0][i] * product;
			result.second += gy[0][i] * product;
		}
		return result;
	}
};