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
};