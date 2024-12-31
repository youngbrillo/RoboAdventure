#include "rayWindow.h"
#include <raylib/raylib.h>

namespace glib
{
    void* rGetWindowHandle()
    {
        return GetWindowHandle();
    }
}