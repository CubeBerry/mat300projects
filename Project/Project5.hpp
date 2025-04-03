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
		t;
		return 0.0;
	}
};