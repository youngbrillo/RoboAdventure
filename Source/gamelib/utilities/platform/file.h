#pragma once
#include <string>

namespace glib
{
    std::string OpenFile(const char* filter);
    std::string SaveFile(const char* filter);
}