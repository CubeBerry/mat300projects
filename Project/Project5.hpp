#pragma once
#include <vector>
#include <cmath>
#include <exponential.hpp>

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

    double DividedDifferences(double t)
	{
        int d = degree;
        double result{ 0.0 };

        for (int i = 0; i < static_cast<int>(controlPoints.size()); ++i)
        {
            // i + d + 1 - i == d + 1
            int n = d + 2;
            std::vector<double> f(n);

			for (int j = 0; j < n; ++j)
			{
				double x = i + j;
				if (t >= x) f[j] = glm::pow(t - x, d);
				else f[j] = 0.0;
			}

			// Compute divided differences
			// final result is f[0]
            for (int j = 1; j < n; ++j)
            {
				for (int k = 0; k < n - j; ++k)
				{
					f[k] = (f[k + 1] - f[k]) / j;
				}
            }

            // (-1)^(d + 1) * (i + d + 1 - i) * [i, ..., i + d + 1](t-x)^d+
            double B = glm::pow(-1.0, d + 1) * (d + 1) * f[0];
			result += B * controlPoints[i];
        }

        return result;
	}

    int Factorial(int n)
	{
		return (n == 0 || n == 1) ? 1 : Factorial(n - 1) * n;
    }

    int BinomialCoefficient(int n, int k)
	{
        if (k > n) return 0;
        if (k == 0 || k == n) return 1;
        return BinomialCoefficient(n - 1, k - 1) + BinomialCoefficient(n - 1, k);
    }

    double SumOfShiftedPowerFunctions(double t)
    {
		double f{ 0.0 };
        int d = degree;
        int n = d + 2;

		for (int i = 0; i < static_cast<int>(controlPoints.size()); ++i)
		{
            double B{ 0.0 };
			// j == [0, d + 1]
			for (int j = 0; j < n; ++j)
			{
                double x = i + j;
                double term = t >= x ? glm::pow(t - x, d) : 0.0;
                double coefficient = glm::pow(-1.0, j) * static_cast<double>(BinomialCoefficient(d + 1, j));
				B += coefficient * term;
			}
			B /= static_cast<double>(Factorial(d));
			f += B * controlPoints[i];
		}
        return f;
    }
};
