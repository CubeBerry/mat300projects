#pragma once
#include <vector>
#include <cmath>

#include "GameState.hpp"

class Project5 : public GameState
{
public:
	Project5() = default;
	~Project5() override {}

	void Init() override;
	void Update(float dt) override;
	void ImGuiDraw(float dt) override;
	void Restart() override;
	void End() override;

private:
	int degree{ 0 };
	int N{ 0 };
	// N - degree - 1
	std::vector<double> controlPoints;

    enum class Method
    {
	    DB,
        DD,
        SSPF
    } method;

	double DeBoor(double t)
	{
        int d = degree;

        if (t < d) t = d;
        if (t >= N - d) t = N - d - 1e-6;

        int J = static_cast<int>(std::floor(t));

        std::vector<double> c = controlPoints;
        for (int k = 1; k <= d; ++k)
        {
            for (int i = J; i >= J - d + k; --i)
            {
                // i + d - k + 1 - i == d - k + 1
                double alpha = ((i + d - k + 1) - t) / (d - k + 1);
                double beta = (t - i) / (d - k + 1);
                c[i] = alpha * c[i - 1] + beta * c[i];
            }
        }
        return c[J];
	}
};