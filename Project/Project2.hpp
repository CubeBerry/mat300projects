#pragma once
#include <vector>
#include <cmath>

#include "GameState.hpp"

class Project2 : public GameState
{
public:
	Project2() = default;
	~Project2() override {}

	void Init() override;
	void Update(float dt) override;
	void ImGuiDraw(float dt) override;
	void Restart() override;
	void End() override;
private:
	enum class METHOD
	{
		NLI,
		BB,
		MIDPOINT
	};
	METHOD method{ METHOD::NLI };

	std::vector<std::pair<double, double>> controlPoints = {
		{ 0.5, 0.5 }
	};

	//1. NLI
	std::pair<double, double> DeCasteljau(double t)
	{
		std::vector<std::pair<double, double>> temp = controlPoints;

		for (size_t i = 1; i < controlPoints.size(); ++i)
		{
			for (size_t j = 0; j < controlPoints.size() - i; ++j)
			{
				temp[j].first = (1.0 - t) * temp[j].first + t * temp[j + 1].first;
				temp[j].second = (1.0 - t) * temp[j].second + t * temp[j + 1].second;
			}
		}

		return temp[0];
	}

	float shellT{ 0.5f };
	std::vector<std::vector<std::pair<double, double>>> DeCasteljauShells(double t)
	{
		std::vector<std::vector<std::pair<double, double>>> shells;
		std::vector<std::pair<double, double>> temp = controlPoints;

		for (size_t i = 1; i < controlPoints.size(); ++i)
		{
			for (size_t j = 0; j < controlPoints.size() - i; ++j)
			{
				temp[j].first = (1.0 - t) * temp[j].first + t * temp[j + 1].first;
				temp[j].second = (1.0 - t) * temp[j].second + t * temp[j + 1].second;
			}

			shells.push_back(std::vector<std::pair<double, double>>(temp.begin(), temp.begin() + (controlPoints.size() - i)));
		}

		return shells;
	}

	//2. BB
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
			p.second += controlPoints[i].second  * bernstein;
		}

		return p;
	}

	//3. Midpoint Subdivision
	std::vector<std::pair<double, double>> MidpointSubdivision(const std::vector<std::pair<double, double>>& cp, int divisionCount)
	{
		if (divisionCount <= 1) return controlPoints;

		int size = static_cast<int>(cp.size());
		double t = 0.5;
		std::vector<std::vector<std::pair<double, double>>> shells(size);
		shells[0] = cp;

		for (size_t i = 1; i < size; ++i)
		{
			shells[i].resize(size - i);
			for (size_t j = 0; j < size - i; ++j)
			{
				//From (1.0 - t) * temp[j].first + t * temp[j + 1].first; since t == 0.5, (1.0 - t) == t == 0.5
				shells[i][j] = { t * (shells[i - 1][j].first + shells[i - 1][j + 1].first),t * (shells[i - 1][j].second + shells[i - 1][j + 1].second) };
			}
		}

		std::vector<std::pair<double, double>> l;
		for (int i = 0; i < size; ++ i)
		{
			l.push_back(shells[i][0]);
		}

		std::vector<std::pair<double, double>> r;
		for (int i = 0; i < size; ++i)
		{
			r.push_back(shells[size - 1 - i][i]);
		}

		std::vector<std::pair<double, double>> m(l);
		for (int i = 1; i < shells.size(); ++i)
		{
			m.push_back(r[i]);
		}

		if (divisionCount == 2) return m;

		auto left = MidpointSubdivision(l, divisionCount - 1);
		auto right = MidpointSubdivision(r, divisionCount - 1);

		left.pop_back();
		left.insert(left.end(), right.begin(), right.end());
		return left;
	}
};