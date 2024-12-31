#pragma once
#include <raylib/raylib.h>



namespace aq::util
{

    void DrawTextBoxed(Font font, const char* text, Rectangle rec, float fontSze, float spacing, bool wordWrap, Color tint);
    void DrawTextBoxedSelectable(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);
}