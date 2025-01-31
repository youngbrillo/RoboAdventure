#define RAYGUI_IMPLEMENTATION
#include "scenes/sceneManager.h"
#include "scenes/tdScene.h"
#include <gamelib/gameLib.h>

int main(int argc, char** argv)
{
	//test::RunGUITests();
	//test::RunMenuTests();
	//test::RunCombatTest();
	SetConfigFlags(0x04);
	SetTraceLogLevel(TraceLogLevel::LOG_WARNING);
	InitWindow(1080, 720, GetFileNameWithoutExt(argv[0]));
	SetWindowMonitor(2);
	SetWindowSize(1080, 720);
	SetTraceLogLevel(TraceLogLevel::LOG_ALL);
	rlImGuiSetup(true);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
	glib::io::InitInputMap();

	td::TowerDefenseScene* scene = new td::TowerDefenseScene(DARKBLUE);
	scene->Set();
	while (!WindowShouldClose())
	{
		//want reset?
		if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_R))
		{
			//want complete restart?
			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				scene->Unset();
				Color lc = scene->backgroundColor;
				delete scene;
				scene = new td::TowerDefenseScene(lc);
				scene->Set();
			}
			else {
				scene->Unset();
				scene->Set();
			}
		}


		float dt = GetFrameTime();
		scene->PollInputs();
		scene->Update(dt);
		BeginDrawing();
		ClearBackground(scene->backgroundColor);
		scene->Draw();
		rlImGuiBegin();
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_::ImGuiDockNodeFlags_PassthruCentralNode);
		scene->Inspect();
		rlImGuiEnd();
		EndDrawing();
	}
	scene->Unset();
	rlImGuiShutdown();
	CloseWindow();


	return 0;
}