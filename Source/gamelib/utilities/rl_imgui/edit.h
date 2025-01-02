#pragma once
#include <raylib/raylib.h>
#include <imgui.h>

namespace ImGui
{
    inline bool ColorEditRay(const char* label, Color* color, ImGuiColorEditFlags flags = ImGuiColorEditFlags_DisplayHex)
    {
        Vector4 v = ColorNormalize(*color);

        if(ImGui::ColorEdit4(label, &v.x, flags))
        {
            *color = ColorFromNormalized(v);
        }
        return false;
    }

    inline bool Camera3DEdit(const char* label, Camera3D* camera)
    {
        bool a = false, b = false, c = false, d = false, e = false;
        if (ImGui::TreeNode(label))
        {
            a = ImGui::DragFloat3("position", &camera->position.x);
            b = ImGui::DragFloat3("target", &camera->target.x);
            c = ImGui::DragFloat3("up", &camera->up.x);
            d = ImGui::SliderFloat("fovy", &camera->fovy, 0.0f, 180.0f);
            e = ImGui::SliderInt("Projection", &camera->projection,
                CameraProjection::CAMERA_PERSPECTIVE, CameraProjection::CAMERA_ORTHOGRAPHIC,
                camera->projection == CAMERA_PERSPECTIVE ? "Perspective" : "Orthographic"
            );

            ImGui::TreePop();
        }
        return a || b || c || d || e ;
    }

    inline void ViewFPS(bool* view, int& stat_position)
    {
		//if (!viewStats) return;
		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		if (stat_position != -1)
		{
			const float PAD = 10.0f;
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = viewport->WorkSize;
			ImVec2 window_pos, window_pos_pivot;
			window_pos.x = (stat_position & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
			window_pos.y = (stat_position & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
			window_pos_pivot.x = (stat_position & 1) ? 1.0f : 0.0f;
			window_pos_pivot.y = (stat_position & 2) ? 1.0f : 0.0f;
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			window_flags |= ImGuiWindowFlags_NoMove;
		}
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		if (ImGui::Begin("Status Menu", view, window_flags))
		{
			//IMGUI_DEMO_MARKER("Examples/Simple Overlay");
			//ImGui::Text("Staus Menu overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
			ImGui::Text("%.3f ms/frame (%04.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			//ImGui::Text("Viewport is: %s", this->mouseOnViewport ? "Hovered" : "Not Hovered");
			//ImGui::Text("State: %s", xlib::Application::Get()->IsPaused() ? "Paused" : "Running");
			ImGui::Separator();
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Custom", NULL, stat_position == -1)) stat_position = -1;
				if (ImGui::MenuItem("Top-left", NULL, stat_position == 0)) stat_position = 0;
				if (ImGui::MenuItem("Top-right", NULL, stat_position == 1)) stat_position = 1;
				if (ImGui::MenuItem("Bottom-left", NULL, stat_position == 2)) stat_position = 2;
				if (ImGui::MenuItem("Bottom-right", NULL, stat_position == 3)) stat_position = 3;
				if (ImGui::MenuItem("Close")) *view = false;
				ImGui::EndPopup();
			}
		}
		ImGui::End();
    }
}
