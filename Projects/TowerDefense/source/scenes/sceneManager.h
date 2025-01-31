#pragma once
#include <vector>
#include <string>
#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <functional>
#include <imgui.h>
#include <entt/entt.hpp>
#include <map>

namespace td
{
	class scene
	{
	public:
		std::string name;
		Color backgroundColor = BLACK;
		bool paused = false, isInspecting = true, isFPSDebug = true;
		int fpsDebugPos = 0;
		entt::registry ecs;

	public:
		scene(std::string Name, Color color = BLACK) : name(Name), backgroundColor(color) {}
		virtual ~scene() {};

		virtual void Set() {}
		virtual void Unset() {}
		virtual void Update(float dt) {}
		virtual void FixedUpdate(float timeStep) {};
		virtual void Draw() {};
		//void InspectWrapper();
		virtual void Inspect() {};
	};

	typedef std::function<scene* ()> sceneGenerationFunction;

	struct SceneEntry
	{
		std::string name, category;
		sceneGenerationFunction generator;
		int id = 0;
	};

	class SceneManager
	{
		static SceneManager* instance;
	public:
		std::vector<SceneEntry> entries;
		std::map<std::string, std::vector<int>> categoryLookups;
		int currentIndex = 0;
		int defaultIndex = 0;
		bool wantNewTest = false;
		bool wantRestartTest = false;
		scene* ActiveTest = nullptr;
	public:
		SceneManager();
		~SceneManager();
		static SceneManager& Get();
		static int AddEntry(std::string name, std::string category, sceneGenerationFunction func);
		static scene* GetDefaultTest();
		static scene* GetTestAt(int index);
		void listenforInputs();
		void Update(float dt);
		void FixedUpdate(float timeStep);
		void Draw();
		void Inspect();
		void DrawMenu(scene* active);

		static void SaveSettings(const char* title = "scene-config.yaml");
		static void LoadSettings(const char* title = "scene-config.yaml");
	};
}