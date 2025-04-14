#include "Engine.hpp"
#include "Project1.hpp"
#include "Project2.hpp"
#include "Project3.hpp"
#include "Project4.hpp"
#include "Project5.hpp"
#include "Project6.hpp"
#include "Project7.hpp"
#include "ProjectExtra1.hpp"
#include "ProjectExtra2.hpp"

#undef main

int main(void)
{
    Engine& engine = Engine::Instance();
    engine.Init("CubeEngine", 1280, 720, false, WindowMode::NORMAL);

    engine.GetGameStateManager().AddLevel(new Project1);
    engine.GetGameStateManager().AddLevel(new Project2);
    engine.GetGameStateManager().AddLevel(new Project3);
	engine.GetGameStateManager().AddLevel(new Project4);
	engine.GetGameStateManager().AddLevel(new Project5);
	engine.GetGameStateManager().AddLevel(new Project6);
	engine.GetGameStateManager().AddLevel(new Project7);
	engine.GetGameStateManager().AddLevel(new ProjectExtra1);
	engine.GetGameStateManager().AddLevel(new ProjectExtra2);
    engine.GetGameStateManager().LevelInit(GameLevel::PROJECT7);

    engine.Update();
    engine.End();
}