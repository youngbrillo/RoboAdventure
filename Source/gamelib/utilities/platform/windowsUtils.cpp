#include "file.h"
#include <Windows.h>
#include <commdlg.h>

#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>
#include "rayWindow.h"

namespace glib::filesystem
{
	std::string OpenFile(const char* filter)
	{
		//capture the current state, so we can pause the app without interruption when we resume
		//int originalState = Application::Settings().UpdateState;
		//Application::Global().Pause();

		OPENFILENAMEA ofn;				//common dialog box structure
		CHAR szFile[260] = { 0 };		//if using TCHAR macros
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		

		ofn.hwndOwner = (HWND)glib::rGetWindowHandle(); // glfwGetWin32Window(win);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;


		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			//Application::Global().SetTimeState((PlayState)originalState);
			return ofn.lpstrFile;
		}
		//Application::Global().SetTimeState((PlayState)originalState);

		return  std::string();
	}
	std::string SaveFile(const char* filter)
	{
		//capture the current state, so we can pause the app without interruption when we resume
		//int originalState = Application::Settings().UpdateState;
		//Application::Global().Pause();

		OPENFILENAMEA ofn;				//common dialog box structure
		CHAR szFile[260] = { 0 };		//if using TCHAR macros
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		


		ofn.hwndOwner = (HWND)glib::rGetWindowHandle(); // glfwGetWin32Window(win);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			//Application::Global().SetTimeState((PlayState)originalState);
			return ofn.lpstrFile;
		}

		//Application::Global().SetTimeState((PlayState)originalState);
		return  std::string();
	}
}