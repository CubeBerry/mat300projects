#include "Engine.hpp"

void Engine::Init(const char* title, int windowWidth, int windowHeight, bool fullScreen, WindowMode mode)
{
	window.Init(title, windowWidth, windowHeight, fullScreen, mode);
	renderManager = new GLRenderManager;
	dynamic_cast<GLRenderManager*>(renderManager)->Initialize(
		window.GetWindow(), window.GetContext()
	);
}

void Engine::Update()
{
	//SDL_Event event;
	while (gameStateManger.GetGameState() != State::SHUTDOWN)
	{
		gameStateManger.Update(deltaTime);
	}
}

void Engine::End()
{
	delete dynamic_cast<GLRenderManager*>(renderManager);
}
