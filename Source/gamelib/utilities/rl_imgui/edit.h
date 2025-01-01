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

}
