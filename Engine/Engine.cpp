#include "Engine.hpp"

#include "backends/imgui_impl_sdl2.h"

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
	SDL_Event event;
	while (gameStateManger.GetGameState() != State::SHUTDOWN)
	{
		SDL_PollEvent(&event);
		ImGui_ImplSDL2_ProcessEvent(&event);
		gameStateManger.Update(deltaTime);

		switch (event.type)
		{
		case SDL_QUIT:
			Engine::GetGameStateManager().SetGameState(State::UNLOAD);
			break;
		case SDL_WINDOWEVENT:
			break;
		default:
			break;
		}
		Engine::GetWindow().UpdateWindowGL(event);
	}
}

void Engine::End()
{
	delete dynamic_cast<GLRenderManager*>(renderManager);
}
