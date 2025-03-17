#pragma once
#include <vector>
#include <cmath>

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

	std::pair<double, double> CubicSpline(const double& t) const
	{
        int N = static_cast<int>(controlPoints.size());
        if (N == 1) return controlPoints[0];

		// [a0, a1, a3, b1, b2, ..., bN-2]
		// a2 == 0 because f''(0) == 0 == 2a2
		int M = N + 1;
		// Coefficients Matrix
		std::vector<std::vector<double>> A(M, std::vector<double>(M, 0.0));
		std::vector<double> rhsX(M, 0.0), rhsY(M, 0.0);

		// Row 0 (t = 0), f(0) = a0
		A[0][0] = 1.0;
		rhsX[0] = controlPoints[0].first;
		rhsY[0] = controlPoints[0].second;

		// Row 1 (t = 1), f(1) = a0 + a1 + a3
		A[1][0] = 1.0;
		A[1][1] = 1.0;
		A[1][2] = 1.0;
		rhsX[1] = controlPoints[1].first;
		rhsY[1] = controlPoints[1].second;

		// Row i (t = i), f(i) = a0 + a1*i + a3*i^3
		for (int i = 2; i < N; ++i)
		{
			A[i][0] = 1.0;
			A[i][1] = i;
			A[i][2] = i * i * i;
			// (t - j)^3 -> (i - j)^3
			for (int j = 1; j < i; ++j) A[i][j + 2] = std::pow(i - j, 3);
			rhsX[i] = controlPoints[i].first;
			rhsY[i] = controlPoints[i].second;
		}

		// Row N (t = N - 1)
		// f''(t) = 6a3 * t, f''(0) = 0, f''(N - 1) = 0
		A[N][0] = 0.0;
		A[N][1] = 0.0;
		A[N][2] = N - 1.0;
		for (int j = 1; j <= N - 2; ++j) A[N][j + 2] = N - 1 - j;
		rhsX[N] = 0.0;
		rhsY[N] = 0.0;
	}
};