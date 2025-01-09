#pragma once
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
	float x_data[5] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
	float y_data[5] = { 2.0f, 3.0f, 5.0f, 7.0f, 11.0f };
};