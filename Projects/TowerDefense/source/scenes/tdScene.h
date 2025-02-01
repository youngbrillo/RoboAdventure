#pragma once
#include "sceneManager.h"
#include "../components/com_core.h"

namespace td
{
	constexpr int max_entities = 400;

	class TowerDefenseScene : public scene
	{
		TowerSystem towers;
		EnemySystem enemies;
		ProjectileSystem projectiles;
	public:
		TowerDefenseScene(Color color = BLACK);
		virtual ~TowerDefenseScene();
		virtual void PollInputs();
		virtual void Set() override;
		virtual void Unset()override;
		virtual void Update(float dt) override;
		virtual void FixedUpdate(float timeStep)override;
		virtual void Draw() override;
		virtual void Inspect() override;

	public:
		Camera3D camera = {};
		entt::registry ecs;
	};
}