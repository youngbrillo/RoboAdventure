#include "cameras.h"
#include "../../utilities/rl_imgui/edit.h"

void glib::Camera3DController::Update(float dt)
{
}

void glib::Camera3DController::Inspect(const char* label)
{
	if (ImGui::TreeNode(label))
	{
		ImGui::SliderFloat("follow speed", &follow_speed, 1.0f, 10.0f);
		ImGui::SliderFloat("target Distance", &targetDistance, 1.0f, 30.0f);
		ImGui::InputFloat2("angle", &angle.x);
		ImGui::InputFloat2("_prevMousePosition", &_prevMousePosition.x);
		ImGui::Camera3DEdit("camera", &camera);

		ImGui::TreePop();
	}
}
