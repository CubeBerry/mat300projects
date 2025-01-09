#include "Engine.hpp"
#include "Project1.hpp"

#undef main

int main(void)
{
    Engine& engine = Engine::Instance();
    engine.Init("CubeEngine", 1280, 720, false, WindowMode::NORMAL);

    engine.GetGameStateManager().AddLevel(new Project1);
    //engine.GetGameStateManager().AddLevel(new VerticesDemo);
    //engine.GetGameStateManager().AddLevel(new PhysicsDemo);
    //engine.GetGameStateManager().AddLevel(new PocketBallDemo);
    //engine.GetGameStateManager().AddLevel(new PlatformDemo);
    //engine.GetGameStateManager().AddLevel(new BeatEmUpDemo);
    engine.GetGameStateManager().LevelInit(GameLevel::PROJECT1);

    engine.Update();
    engine.End();
}