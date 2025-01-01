#define RAYGUI_IMPLEMENTATION
#include "tests/test.h"
#include <gamelib/gameLib.h>
int main(int argc, char** argv)
{
	//test::RunGUITests();
	//test::RunMenuTests();
	//test::RunCombatTest();
	SetConfigFlags(0x04);
	SetTraceLogLevel(TraceLogLevel::LOG_WARNING);
	InitWindow(1080, 720, "Quest: Testsuite");
	SetWindowMonitor(2);
	SetWindowSize(1080, 720);
	SetTraceLogLevel(TraceLogLevel::LOG_ALL);

	testbed::TestManager& manager = testbed::TestManager::Get();
	manager.wantNewTest = true;
	manager.LoadSettings();
	rlImGuiSetup(true);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;

	glib::io::InitInputMap();

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		manager.listenforInputs();
		manager.Update(dt);

		BeginDrawing();
		manager.Draw();

		rlImGuiBegin();
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_::ImGuiDockNodeFlags_PassthruCentralNode);
		manager.Inspect();
		rlImGuiEnd();
		EndDrawing();
	}
	if (manager.ActiveTest) { manager.ActiveTest->Unset(); delete manager.ActiveTest; manager.ActiveTest = nullptr; }
	manager.SaveSettings();
	rlImGuiShutdown();
	CloseWindow();


	return 0;
}