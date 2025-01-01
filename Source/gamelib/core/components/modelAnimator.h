#pragma once

#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <string>
#include <vector>


namespace glib
{
	struct ModelAnimator
	{
		int count = 0; //animation count
		int index = 0; //animation index
		int currentFrame = 0; //current animation frame
		int defaultAnimation = 0;
		int nextAnimation = 0;
		float timer = 0, animSpeed = 1.0f / 60.0f;
		bool frameEnded = false;
		ModelAnimation* animations = nullptr;

		void Load(const char* filename);
		void UnLoad();
		void Update(float dt, Model& model);
		void SetAnimation(int id);
		void SetAnimationBlend(int id);
		void SetAnimationName(const char* tile);
		int GetAnimation(const char* title);
		bool inProgress() const;
		void inspect(const char* title = "Animator");
	};

	struct AnimationQueue
	{
		std::vector<int> queue;

		AnimationQueue& pushAnimation(ModelAnimator& model, const char* label);
		AnimationQueue& pushAnimation(ModelAnimator& model, int id);
		void Update(ModelAnimator& animator, Model* model = nullptr);
	};
}