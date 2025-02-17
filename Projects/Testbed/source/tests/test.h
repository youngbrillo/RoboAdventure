#pragma once
#include <vector>
#include <string>
#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <functional>
#include <imgui.h>
#include <entt/entt.hpp>
#include <map>

namespace testbed
{
	class Test
	{
	public:
		std::string name;
		Color backgroundColor = BLACK;
		bool paused = false, isInspecting = true, isFPSDebug = true;
		int fpsDebugPos = 0;
		entt::registry ecs;

	public:
		Test(std::string Name, Color color = BLACK) : name(Name), backgroundColor(color) {}
		virtual ~Test() {};

		virtual void Set() {}
		virtual void Unset() {}
		virtual void Update(float dt) {}
		virtual void FixedUpdate(float timeStep) {};
		virtual void Draw() {};
		void InspectWrapper();
		virtual void Inspect() {};
	};

	typedef std::function<Test* ()> TestGenerationFunction;
	struct TestEntry
	{
		std::string name, category;
		TestGenerationFunction generator;
		int id = 0;
	};

	class TestManager
	{
		static TestManager* instance;
	public:
		std::vector<TestEntry> entries;
		std::map<std::string, std::vector<int>> categoryLookups;
		int currentIndex = 0;
		int defaultIndex = 0;
		bool wantNewTest = false;
		bool wantRestartTest = false;
		Test* ActiveTest = nullptr;
	public:
		TestManager();
		~TestManager();
		static TestManager& Get();
		static int AddEntry(std::string name, std::string category, TestGenerationFunction func);
		static Test* GetDefaultTest();
		static Test* GetTestAt(int index);
		void listenforInputs();
		void Update(float dt);
		void FixedUpdate(float timeStep);
		void Draw();
		void Inspect();
		void DrawMenu(Test* active);

		static void SaveSettings(const char* title = "test-config.yaml");
		static void LoadSettings(const char* title = "test-config.yaml");
	};
}

/*

#pragma once
#include "test.h"

namespace testbed
{
	class Template : public Test
	{
	public:
		Template()
			: Test("Template", BLACK)
		{

		}

		~Template()
		{

		}

		virtual void Set()
		{

		}
		virtual void Unset()
		{

		}

		virtual void Update(float dt)
		{

		}
		virtual void FixedUpdate(float timeStep)
		{

		};
		virtual void Draw()
		{
			DrawText(name.c_str(), 20, GetScreenHeight() - 40, 20, WHITE);
		};
		virtual void Inspect()
		{
			ImGui::Checkbox("paused", &this->paused);
		};
		static Test* Generate() { return new Template(); }
	};

	static int entry = testbed::TestManager::AddEntry("Template_Name", "Cateogry", testbed::Template::Generate);
}

*/