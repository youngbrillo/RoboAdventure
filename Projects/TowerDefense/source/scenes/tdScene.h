#pragma once

#include "sceneManager.h"

namespace td
{
	class TowerDefenseScene : public scene
	{
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
	};
}