#pragma once
#include "Window.hpp"
#include "GLRenderManager.hpp"
#include "GameStateManager.hpp"

class Engine
{
public:
	Engine() = default;
	~Engine() = default;

	static Engine& Instance() { static Engine instance; return instance; }
	static Window& GetWindow() { return Instance().window; }
	static RenderManager* GetRenderManager() { return Instance().renderManager; }

	void Init(const char* title, int windowWidth, int windowHeight, bool fullScreen, WindowMode mode);
	void Update();
	void End();

	void ResetDeltaTime();
private:
	float deltaTime = 0.f;

	Window window;
	RenderManager* renderManager;
	GameStateManager gameStateManger;
};