#pragma once
#include <vector>
#include <cmath>

#include "GameState.hpp"

class Project7 : public GameState
{
public:
	Project7() = default;
	~Project7() override {}

	void Init() override;
	void Update(float dt) override;
	void ImGuiDraw(float dt) override;
	void Restart() override;
	void End() override;
private:
	int degree{ 3 };
	std::vector<std::pair<double, double>> controlPoints = {
		{ 0.5, 0.5 },
		{ 0.5, 0.5 },
		{ 0.5, 0.5 },
		{ 0.5, 0.5 },
	};
	std::vector<double> knotSequence;
	double shellT{ 3.0 };

	void UpdateKnotSequence()
	{
		int s = static_cast<int>(controlPoints.size()) - 1;
		int N = s + degree + 1;
		knotSequence.resize(N + 1);
		for (int i = 0; i < N + 1; ++i)
		{
			knotSequence[i] = static_cast<double>(i);
		}
		double tMin{ knotSequence[degree] };
		double tMax{ knotSequence[N - degree] };
		if (shellT < tMin) shellT = tMin;
		if (shellT > tMax) shellT = tMax;
	}

	std::pair<double, double> DeBoor(double t) const
	{
		if (degree < 0) return { 0.0, 0.0 };

		int s = static_cast<int>(controlPoints.size()) - 1;
		int N = s + degree + 1;

		int J = degree;
		for (int j = degree; j < N - degree; ++j)
		{
			if (t >= knotSequence[j] && t < knotSequence[j + 1])
			{
				J = j;
				break;
			}
		}
		if (t >= knotSequence[N - degree]) J = s;

		//int offset = J - degree;

		std::vector<std::pair<double, double>> P(s + 1);
		for (int i = 0; i <= s; ++i)
		{
			P[i] = controlPoints[i];
		}

		for (int p = 1; p <= degree; ++p)
		{
			for (int i = J - degree + p; i <= J; ++i)
			{
				// i + d - p + 1 - i == d - p + 1
				double denominator = knotSequence[i + degree - p + 1] - knotSequence[i];
				double alpha = (t - knotSequence[i]) / denominator;
				double beta = (knotSequence[i + degree - p + 1] - t) / denominator;
				//c[i] = alpha * c[i - 1] + beta * c[i];
				P[i].first = alpha * P[i].first + beta * P[i - 1].first;
				P[i].second = alpha * P[i].second + beta * P[i - 1].second;
			}
		}

		return P[J];
	}
};