#pragma once
#include <vector>
#include <cmath>

#include "GameState.hpp"

class Project1 : public GameState
{
public:
	Project1() = default;
	~Project1() override {}

	void Init() override;
	void Update(float dt) override;
	void ImGuiDraw(float dt) override;
	void Restart() override;
	void End() override;

private:
	int degree{ 1 };
	std::vector<double> a{ 1.0, 1.0 };
	bool isDeCasteljau{ true };

	static int BinomialCoefficient(int n, int k)
	{
		int result{ 1 };

		if (k > n - k) k = n - k;

		for (int i = 0; i < k; ++i)
		{
			result *= (n - i);
			result /= (i + 1);
		}
		return result;
	}

	double BBForm(const std::vector<double>& cp, double t)
	{
		int degree = static_cast<int>(cp.size()) - 1;

		double p{ 0.0 };
		for (int i = 0; i <= degree; ++i)
		{
			double coefficient = BinomialCoefficient(degree, i);
			double bernstein = coefficient * std::pow(1.0 - t, degree - i) * std::pow(t, i);
			p += cp[i] * bernstein;
		}

		return p;
	}

	double DeCasteljau(const std::vector<double>& cp, double t)
	{
		int degree = static_cast<int>(cp.size()) - 1;

		std::vector<double> temp(cp.begin(), cp.end());

		for (int i = 1; i <= degree; ++i)
		{
			for (int j = 0; j <= degree - i; ++j)
			{
				temp[j] = (1.0 - t) * temp[j] + t * temp[j + 1];
			}
		}

		return temp[0];
	}
};