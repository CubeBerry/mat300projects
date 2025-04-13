#pragma once
#include <vector>

#include "GameState.hpp"

using Point = std::pair<double, double>;

struct HermitePoint
{
	// control points
	Point p;
	// tangent vectors
	Point t;
    bool isShowTangent;
};

class ProjectExtra1 : public GameState
{
public:
	ProjectExtra1() = default;
	~ProjectExtra1() override {}

	void Init() override;
	void Update(float dt) override;
	void ImGuiDraw(float dt) override;
	void Restart() override;
	void End() override;
private:
    std::vector<HermitePoint> controlPoints;

	Point HermiteInterpolation(double t) const
	{
		size_t n = controlPoints.size();
		if (n < 2) return { 0.0, 0.0 };

		double segmentLength = 1.0 / static_cast<double>(n - 1);
		// segment == floor(t * (n - 1))
		size_t segment = static_cast<size_t>(t / segmentLength);
		if (segment >= n - 1) segment = n - 2;
		double local_t = (t - static_cast<double>(segment) * segmentLength) / segmentLength;

		double h00 = 2.0 * local_t * local_t * local_t - 3.0 * local_t * local_t + 1.0;
		double h10 = local_t * local_t * local_t - 2.0 * local_t * local_t + local_t;
		double h01 = -2.0 * local_t * local_t * local_t + 3.0 * local_t * local_t;
		double h11 = local_t * local_t * local_t - local_t * local_t;

		double x = h00 * controlPoints[segment].p.first + h10 * controlPoints[segment].t.first + h01 * controlPoints[segment + 1].p.first + h11 * controlPoints[segment + 1].t.first;
		double y = h00 * controlPoints[segment].p.second + h10 * controlPoints[segment].t.second + h01 * controlPoints[segment + 1].p.second + h11 * controlPoints[segment + 1].t.second;

		return { x,y };
	}
};