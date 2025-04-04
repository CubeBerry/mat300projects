#include "Window.hpp"
#include "glew.h"

#include <iostream>

void Window::Init(const char* title, int width, int height, bool fullscreen, WindowMode wMode)
{
	int flags = 0;
	wSize.x = (float)width;
	wSize.y = (float)height;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
	}
	else
	{
		InitWindowGL(wMode, title, flags);

		SetMainWindowTitle(title);
		std::cout << "Create Window Successful" << '\n' << std::endl;
	}
}

void Window::InitWindowGL(WindowMode wMode, const char* title, int flags)
{
	//Use Modern OpenGL Code Only
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//Use OpenGL Version 4.6
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	//Use Double Buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	//MultiSampling
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	switch (wMode)
	{
	case WindowMode::NORMAL:
		window = std::unique_ptr<SDL_Window, SDLWindowDestroyer>(SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			(int)wSize.x,
			(int)wSize.y,
			flags | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL));
		break;
	case WindowMode::BORADLESS:
		window = std::unique_ptr<SDL_Window, SDLWindowDestroyer>(SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			(int)wSize.x,
			(int)wSize.y,
			flags | SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL));
		break;
	};

	//Make Context
	context = SDL_GL_CreateContext(window.get());
	SDL_GL_MakeCurrent(window.get(), context);

	//Init GLEW
	GLenum result = glewInit();
	if (result != GLEW_OK)
		std::cerr << "GLEW Init Failed: " << glewGetErrorString(result) << std::endl;

	//VSYNC
	//constexpr int ADAPTIVE_VSYNC = -1;
	//constexpr int VSYNC = 1;
	//if (const auto result = SDL_GL_SetSwapInterval(ADAPTIVE_VSYNC); result != 0)
	//{
	//	SDL_GL_SetSwapInterval(VSYNC);
	//}

	//Anti-Aliasing
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//MSAA Anti-Aliasing
	glEnable(GL_MULTISAMPLE);

	//Blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::UpdateWindowGL(SDL_Event& event)
{
	if ((event.type == SDL_WINDOWEVENT) && (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED))
	{
		glViewport(0, 0, event.window.data1, event.window.data2);
	}
	//SDL_GL_SwapWindow(window.get());
}

void Window::SetMainWindowTitle(std::string title)
{
	titleName = title;
	SDL_SetWindowTitle(window.get(), titleName.c_str());
}

void Window::SetSubWindowTitle(std::string subTitle)
{
	SDL_SetWindowTitle(window.get(), (titleName + subTitle).c_str());
}
