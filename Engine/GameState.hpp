#pragma once

class GameState
{
public:
	virtual ~GameState() {}

	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void ImGuiDraw(float dt) = 0;
	virtual void Restart() = 0;
	virtual void End() = 0;
protected:
};