#include "common.h"
#include <imgui.h>
#include "../../utilities/rl_imgui/edit.h"

void glib::Transform3D::Inspect(const char* label)
{
	if (ImGui::TreeNode(label))
	{
		ImGui::DragFloat3("translation", &translation.x);
		ImGui::DragFloat4("rotation", &rotation.x);
		ImGui::DragFloat3("scale", &scale.x);


		ImGui::TreePop();
	}
}

void glib::Velocity3D::Inspect(const char* label)
{
	ImGui::DragFloat3(label, &this->x);

	//if (ImGui::TreeNode(label))
	//{

	//	ImGui::TreePop();
	//}
}

void glib::Model3D::Inspect(const char* label)
{
	if (ImGui::TreeNode(label))
	{
		ImGui::ColorEditRay("tint", &tint);

		ImGui::TreePop();
	}
}
