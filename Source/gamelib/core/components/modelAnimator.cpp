#include "modelAnimator.h"
#include <imgui.h>
void glib::ModelAnimator::Load(const char* filename)
{
	if (animations)
	{
		UnloadModelAnimations(animations, count);
	}
	animations = LoadModelAnimations(filename, &count);
	SetAnimation(defaultAnimation);
}

void glib::ModelAnimator::UnLoad()
{
	if (animations)
	{
		UnloadModelAnimations(animations, count);
	}
	animations = nullptr;
	count = 0;
}

void glib::ModelAnimator::Update(float dt, Model& model)
{
	frameEnded = false;
	if (!animations)
		return;
	timer += dt;
	if (timer >= animSpeed)
	{
		timer = 0;
		ModelAnimation anim = animations[index];
		if (index != nextAnimation && (currentFrame + 1) >= anim.frameCount)
		{
			index = nextAnimation;
			nextAnimation = index;
			anim = animations[index];
			currentFrame = -1;
			//frameEnded = true;
		}
		frameEnded = ((currentFrame + 1) == anim.frameCount);
		currentFrame = (currentFrame + 1) % anim.frameCount;
		UpdateModelAnimation(model, anim, currentFrame);
	}
}

void glib::ModelAnimator::SetAnimation(int id)
{
	if (id < 0 || id >= count)
		return;
	if (id == index)
		return;
	index = id;
	nextAnimation = id;
	timer = 0.0f;
	currentFrame = 0;
}

void glib::ModelAnimator::SetAnimationBlend(int id)
{
	if (id < 0 || id >= count)
		return;

	nextAnimation = id;
}

void glib::ModelAnimator::SetAnimationName(const char* label)
{
	for (int i = 0; i < count; i++)
	{
		if (TextIsEqual(animations[i].name, label))
		{
			SetAnimation(i);
		}
	}
}

int glib::ModelAnimator::GetAnimation(const char* label)
{
	for (int i = 0; i < count; i++)
	{
		if (TextIsEqual(animations[i].name, label))
		{
			return i;
		}
	}
	return -1;
}

bool glib::ModelAnimator::inProgress() const
{
	if (index < 0 || index >= count) return false;

	return frameEnded;
}

void glib::ModelAnimator::inspect(const char* title)
{

	if (ImGui::TreeNode(title))
	{
		ImGui::SliderFloat("Animation Speed", &animSpeed, 0.001, 1.0f);
		ImGui::InputInt("animation index", &index);
		ImGui::InputInt("current Frame", &currentFrame);
		if (ImGui::TreeNode("Animations"))
		{
			for (int i = 0; i < count; i++)
			{
				if (ImGui::Selectable(animations[i].name, index == i))
				{
					nextAnimation = i;
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

glib::AnimationQueue& glib::AnimationQueue::pushAnimation(ModelAnimator& model, const char* label)
{
    // TODO: insert return statement here
	return *this;
}

glib::AnimationQueue& glib::AnimationQueue::pushAnimation(ModelAnimator& model, int id)
{
    // TODO: insert return statement here

	return *this;
}

void glib::AnimationQueue::Update(ModelAnimator& animator, Model* model)
{
}
