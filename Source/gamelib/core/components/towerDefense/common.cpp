#include "common.h"
/*
*/
namespace glib::towerdefense
{
    void DrawHealthBar(Camera3D camera, Vector3 position, float healthRatio, Color barColor)
    {
        const float healthBarWidth = 40.0f;
        const float healthBarHeight = 6.0f;
        const float healthBarOffset = 15.0f;
        const float inset = 2.0f;
        const float innerWidth = healthBarWidth - inset * 2;
        const float innerHeight = healthBarHeight - inset * 2;

        Vector2 screenPos = GetWorldToScreen(position, camera);
        float centerX = screenPos.x - healthBarWidth * 0.5f;
        float topY = screenPos.y - healthBarOffset;
        DrawRectangle(centerX, topY, healthBarWidth, healthBarHeight, BLACK);
        float healthWidth = innerWidth * healthRatio;
        DrawRectangle(centerX + inset, topY + inset, healthWidth, innerHeight, barColor);
    }


    bool Button(const char* text, int x, int y, int width, int height, ButtonState* state)
    {
        Rectangle bounds = { x, y, width, height };
        bool isPressed = false;
        bool isSelected = state && state->isSelected;
        if (CheckCollisionPointRec(GetMousePosition(), bounds) && !guiState.isBlocked && (state && !state->isDisabled))
        {
            Color color = isSelected ? DARKGRAY : GRAY;
            DrawRectangle(x, y, width, height, color);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                isPressed = true;
            }
            guiState.isBlocked = 1;
        }
        else
        {
            Color color = isSelected ? WHITE : LIGHTGRAY;
            DrawRectangle(x, y, width, height, color);
        }
        Font font = GetFontDefault();
        Vector2 textSize = MeasureTextEx(font, text, font.baseSize * 2.0f, 1);
        Color textColor = state && state->isDisabled ? GRAY : BLACK;
        DrawTextEx(font, text, Vector2{ x + width / 2 - textSize.x / 2, y + height / 2 - textSize.y / 2 }, font.baseSize * 2.0f, 1, textColor);
        return isPressed;
    }


}
