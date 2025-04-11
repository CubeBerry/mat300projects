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

	bool isNewDefaultKnotSequence{ false };
	std::vector<double> knotSequence;
	std::vector<std::pair<double, double>> deBoorControlPoints;

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

	void UpdateKnowSequence()
	{
		int d = static_cast<int>(controlPoints.size()) - 1;
		knotSequence.clear();

		if (!isNewDefaultKnotSequence)
		{
			for (int t = 0; t < d + 1; ++t) knotSequence.push_back(0.0);
			for (int t = 0; t < d + 1; ++t) knotSequence.push_back(1.0);
		}
		else
		{
			for (int t = 0; t < d + 1; ++t)
			{
				knotSequence.push_back(-(d - t) / 10.0);
			}
			// N + 1 == 2 * d + 2
			for (int t = d + 1; t < 2 * d + 2; ++t)
			{
				// 1, 1 + 1 / 10, 1 + 2 / 10, ..., 1 + (d - 1) / 10
				knotSequence.push_back(1.0 + (t - (d + 1.0)) / 10.0);
			}
		}
	}

	void UpdateDeBoorControlPoints()
	{
		int d = static_cast<int>(controlPoints.size()) - 1;
		deBoorControlPoints.clear();
		// Q0 = F[t1, ..., td] ... Q1 = F[t2, ..., td+1] ... Qd = F[td + 1, ..., t2d]
		for (int i = 0; i < d + 1; ++i)
		{
			double sum = 0.0;
			// if i == d, then j == 2d
			// Polar Form -> Assuming since control point is a single value -> F[t1, ..., td] = (t1 + ... + td) / d
			for (int j = i + 1; j <= i + d; ++j)
			{
				sum += knotSequence[j];
			}
			double average = sum / d;
			deBoorControlPoints.push_back(BBForm(average));
		}
	}

	std::pair<double, double> DeBoor(double t)
	{
		int d = static_cast<int>(deBoorControlPoints.size()) - 1;
		if (d < 0) return { 0.0, 0.0 };

		std::vector<std::vector<std::pair<double, double>>> Q(d + 1, std::vector<std::pair<double, double>>(d + 1));

		for (int i = 0; i <=d; ++i)
		{
			Q[0][i] = deBoorControlPoints[i];
		}

		for (int p = 1; p <= d; ++p)
		{
			for (int i = d; i >= p; --i)
			{
				// i + d - p + 1 - i == d - p + 1
				double denominator = knotSequence[i + d - p + 1] - knotSequence[i];
				double alpha = (t - knotSequence[i]) / denominator;
				double beta = (knotSequence[i + d - p + 1] - t) / denominator;
				//c[i] = alpha * c[i - 1] + beta * c[i];
				Q[p][i].first = alpha * Q[p - 1][i].first + beta * Q[p - 1][i - 1].first;
				Q[p][i].second = alpha * Q[p - 1][i].second + beta * Q[p - 1][i - 1].second;
			}
		}

		return Q[d][d];
	}
};