#include "Project1.hpp"
#include "Engine.hpp"
#include "imgui.h"

void Project1::Init()
{
}

void Project1::Update(float dt)
{
}

void Project1::ImGuiDraw(float /*dt*/)
{
	ImGui::ShowDemoWindow();
}

void Project1::Restart()
{
	End();
	Init();
}

void Project1::End()
{
}
