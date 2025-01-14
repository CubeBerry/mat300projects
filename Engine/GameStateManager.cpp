#include "GameStateManager.hpp"

#include <iostream>

#include "Engine.hpp"

#include "imgui.h"

GameStateManager::GameStateManager()
{
}

GameStateManager::~GameStateManager()
{
	for (auto& lev : levelList)
	{
		delete lev;
	}
	levelList.clear();
}

void GameStateManager::LevelInit()
{
	levelList.at(static_cast<int>(currentLevel))->Init();

	if (state != State::CHANGE)
		levelSelected = currentLevel;
}

void GameStateManager::LevelInit(GameLevel currentLevel_)
{
	currentLevel = currentLevel_;
	LevelInit();
	state = State::UPDATE;
#ifdef _DEBUG
	std::cout << "Load Complete" << std::endl;
#endif
}

void GameStateManager::Update(float dt)
{
	switch (state)
	{
	case State::START:
		if (levelList.empty())
		{
			state = State::SHUTDOWN;
		}
		else
		{
			state = State::LOAD;
		}
		break;
	case State::LOAD:
		LevelInit();
#ifdef _DEBUG
		std::cout << "Load Complete" << std::endl;
#endif
		state = State::UPDATE;
#ifdef _DEBUG
		std::cout << "Update" << std::endl;
#endif
		break;
	case State::UPDATE:
		UpdateGameLogic(dt);
		UpdateDraw(dt);
		//Mouse Input X if order is opposite
		break;
	case State::CHANGE:
		levelList.at(static_cast<int>(currentLevel))->End();
		currentLevel = levelSelected;
#ifdef _DEBUG
		std::cout << "Level Change" << std::endl;
#endif
		state = State::LOAD;
		break;
	case State::RESTART:
		LevelEnd();
		state = State::LOAD;
#ifdef _DEBUG
		std::cout << "Level Restart" << std::endl;
#endif
		break;
	case State::UNLOAD:
		LevelEnd();
		state = State::SHUTDOWN;
#ifdef _DEBUG
		std::cout << "ShutDown" << std::endl;
#endif
		break;
	case State::SHUTDOWN:
		break;
	}
}

void GameStateManager::Draw()
{
	RenderManager* renderManager = Engine::Instance().GetRenderManager();
	renderManager->BeginRender({ 0.0f, 0.0f, 0.0f });
	renderManager->EndRender();
}

void GameStateManager::DrawWithImGui(float dt)
{
	RenderManager* renderManager = Engine::Instance().GetRenderManager();
	renderManager->BeginRender({ 0.0f, 0.0f, 0.0f });
	StateChanger();
	levelList.at(static_cast<int>(currentLevel))->ImGuiDraw(dt);
	renderManager->EndRender();
}

void GameStateManager::LevelEnd()
{
	levelList.at(static_cast<int>(currentLevel))->End();
}

void GameStateManager::AddLevel(GameState* level)
{
	levelList.push_back(level);
}

void GameStateManager::ChangeLevel(GameLevel lev)
{
	levelSelected = lev;
}

void GameStateManager::RestartLevel()
{
	levelList.at(static_cast<int>(currentLevel))->Restart();
}

void GameStateManager::UpdateGameLogic(float dt)
{
	if (state == State::UPDATE)
	{
		if (levelSelected != currentLevel)
		{
			state = State::CHANGE;
		}
		else
		{
			levelList.at(static_cast<int>(currentLevel))->Update(dt);
		}
	}
}
void GameStateManager::UpdateDraw(float dt)
{
	if (state == State::UPDATE)
	{
		if (!(SDL_GetWindowFlags(Engine::GetWindow().GetWindow()) & SDL_WINDOW_MINIMIZED))
		{
			DrawWithImGui(dt);
		}
	}
}
void GameStateManager::StateChanger()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Change Level"))
		{
			for (int i = 0; i < levelList.size(); i++)
			{
				std::string levelName = GameLevelTypeEnumToChar(static_cast<GameLevel>(i));
				if (ImGui::MenuItem(levelName.c_str(), std::to_string(i).c_str(), levelSelected == static_cast<GameLevel>(i)))
				{
					levelSelected = static_cast<GameLevel>(i);
					//showDescription = false;
				}
			}
			ImGui::EndMenu();
		}
		//if (ImGui::BeginMenu("Help"))
		//{
		//	if (ImGui::MenuItem("How To Control"))
		//	{
		//		showDescription = true;
		//	}
		//	ImGui::EndMenu();
		//}

		ImGui::EndMainMenuBar();
	}

	//if (showDescription)
	//{
	//	ImGui::OpenPopup("DescriptionPopup");
	//	showDescription = false;
	//}

	//if (ImGui::BeginPopup("DescriptionPopup"))
	//{
	//	switch (currentLevel)
	//	{
	//	case GameLevel::PROJECT1:
	//		ImGui::Text("PROJECT1                              ");
	//		ImGui::Separator();

	//		ImGui::TextWrapped("Move : Arrow Keys\nMove Up/Down : Space Bar/Left Shift\nMove Camera view: Drag with Mouse Right Click\nYou can adjust the mesh's components via the control panel.");
	//		if (ImGui::Button("Close"))
	//		{
	//			ImGui::CloseCurrentPopup();
	//		}
	//		break;
	//	}
	//	ImGui::EndPopup();
	//}
}

const char* GameStateManager::GameLevelTypeEnumToChar(GameLevel type)
{
	switch (type)
	{
	case GameLevel::PROJECT1:
		return "PROJECT1";
		break;
	//case GameLevel::VERTICES:
	//	return "VERTICES";
	//	break;
	//case GameLevel::PHYSICSDEMO:
	//	return "PHYSICSDEMO";
	//	break;
	//case GameLevel::POCKETBALL:
	//	return "POCKETBALL";
	//	break;
	//case GameLevel::PLATFORMDEMO:
	//	return "PLATFORMDEMO";
	//	break;
	//case GameLevel::BEATEMUPDEMO:
	//	return "BEATEMUPDEMO";
	//	break;
	}
	return "NONE";
}
