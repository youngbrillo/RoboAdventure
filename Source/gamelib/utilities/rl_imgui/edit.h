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
}
