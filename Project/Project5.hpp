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
        for (int p = 1; p <= d; ++p)
        {
            for (int i = J; i >= J - d + p; --i)
            {
                // i + d - p + 1 - i == d - p + 1
                double alpha = ((i + d - p + 1) - t) / (d - p + 1);
                double beta = (t - i) / (d - p + 1);
                c[i] = alpha * c[i - 1] + beta * c[i];
            }
        }
        return c[J];
	}
};