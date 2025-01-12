#pragma once
#include <vector>

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
	std::vector<double> a{2, 1.0};


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