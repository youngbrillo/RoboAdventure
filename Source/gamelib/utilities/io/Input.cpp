#include "Input.h"
#include <map>
#include <vector>
#include <string>

namespace glib::io
{
	static std::map<std::string, std::vector<InputEvent>> event_map;
	static InputSettings s_settings;

	void InitInputMap()
	{
		event_map = {};
		//left
		RegisterInput("left", INPUT_TYPE_KEY, KEY_A);
		RegisterInput("left", INPUT_TYPE_KEY, KEY_LEFT);
		RegisterInput("left", INPUT_TYPE_GAMEPAD_BUTTON, GAMEPAD_BUTTON_LEFT_FACE_LEFT);

		//right
		RegisterInput("right", INPUT_TYPE_KEY, KEY_D);
		RegisterInput("right", INPUT_TYPE_KEY, KEY_RIGHT);
		RegisterInput("right", INPUT_TYPE_GAMEPAD_BUTTON, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);

		//up
		RegisterInput("up", INPUT_TYPE_KEY, KEY_W);
		RegisterInput("up", INPUT_TYPE_KEY, KEY_UP);
		RegisterInput("up", INPUT_TYPE_GAMEPAD_BUTTON, GAMEPAD_BUTTON_LEFT_FACE_UP);

		//down
		RegisterInput("down", INPUT_TYPE_KEY, KEY_S);
		RegisterInput("down", INPUT_TYPE_KEY, KEY_DOWN);
		RegisterInput("down", INPUT_TYPE_GAMEPAD_BUTTON, GAMEPAD_BUTTON_LEFT_FACE_DOWN);

		//confirm
		RegisterInput("confirm", INPUT_TYPE_KEY, KEY_Z);
		RegisterInput("confirm", INPUT_TYPE_KEY, KEY_ENTER);
		RegisterInput("confirm", INPUT_TYPE_GAMEPAD_BUTTON, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);

		//cancel
		RegisterInput("cancel", INPUT_TYPE_KEY, KEY_X);
		RegisterInput("cancel", INPUT_TYPE_KEY, KEY_BACKSPACE);
		RegisterInput("cancel", INPUT_TYPE_GAMEPAD_BUTTON, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);

	}

	void LoadInputs(const char* inputConfigFile)
	{
	}

	void RegisterInput(const char* alias, int type, int value)
	{
		event_map[alias].emplace_back(InputEvent{ .type = type, .value = value });
	}

	bool isActionPressed(const char* alias)
	{
		if (event_map.find(alias) == event_map.end()) return false;

		for (auto& e : event_map[alias])
		{
			switch (e.type)
			{
			case io::INPUT_TYPE_KEY:
				if (IsKeyPressed(e.value)) return true;
			case glib::io::INPUT_TYPE_MOUSE_BUTTON:
				if (IsMouseButtonPressed(e.value)) return true;
			case glib::io::INPUT_TYPE_GAMEPAD_BUTTON:
				if (IsGamepadAvailable(s_settings.gamepad) && IsGamepadButtonPressed(s_settings.gamepad, e.value)) return true;
			case glib::io::INPUT_TYPE_GAMEPAD_JOYSTICK:
			default:
				break;
			}
		}
		return false;
	}

	bool isActionDown(const char* alias)
	{
		if (event_map.find(alias) == event_map.end()) return false;

		for (auto& e : event_map[alias])
		{
			switch (e.type)
			{
			case io::INPUT_TYPE_KEY:
				if (IsKeyDown(e.value)) return true;
			case glib::io::INPUT_TYPE_MOUSE_BUTTON:
				if (IsMouseButtonDown(e.value)) return true;
			case glib::io::INPUT_TYPE_GAMEPAD_BUTTON:
				if (IsGamepadAvailable(s_settings.gamepad) && IsGamepadButtonDown(s_settings.gamepad, e.value)) return true;
			case glib::io::INPUT_TYPE_GAMEPAD_JOYSTICK:
			default:
				break;
			}
		}
		return false;
	}

	bool isActionReleased(const char* alias)
	{
		if (event_map.find(alias) == event_map.end()) return false;

		for (auto& e : event_map[alias])
		{
			switch (e.type)
			{
			case io::INPUT_TYPE_KEY:
				if (IsKeyReleased(e.value)) return true;
			case glib::io::INPUT_TYPE_MOUSE_BUTTON:
				if (IsMouseButtonReleased(e.value)) return true;
			case glib::io::INPUT_TYPE_GAMEPAD_BUTTON:
				if (IsGamepadAvailable(s_settings.gamepad) && IsGamepadButtonReleased(s_settings.gamepad, e.value)) return true;
			case glib::io::INPUT_TYPE_GAMEPAD_JOYSTICK:
			default:
				break;
			}
		}
		return false;
	}

	Vector2 GetMoveDirection()
	{
		Vector2 v;
		v.x = (isActionDown("right") ? 1 : 0) - (isActionDown("left") ? 1 : 0);
		v.y = (isActionDown("down") ? 1 : 0) - (isActionDown("up") ? 1 : 0);

		return v;
	}

	Vector2 GetLeftJoystickDirection()
	{
		Vector2 v = { 0.0f, 0.0f };

		if (!IsGamepadAvailable(s_settings.gamepad))
			return GetMoveDirection();

		float leftStickX = GetGamepadAxisMovement(s_settings.gamepad, GAMEPAD_AXIS_LEFT_X);
		float leftStickY = GetGamepadAxisMovement(s_settings.gamepad, GAMEPAD_AXIS_LEFT_Y);
		// Calculate deadzones
		if (leftStickX > -s_settings.leftStickDeadzoneX && leftStickX < s_settings.leftStickDeadzoneX) leftStickX = 0.0f;
		if (leftStickY > -s_settings.leftStickDeadzoneY && leftStickY < s_settings.leftStickDeadzoneY) leftStickY = 0.0f;

		v.x = leftStickX;
		v.y = leftStickY;

		return v;
	}

	Vector2 GetRightJoystickDirection()
	{
		Vector2 v = { 0.0f, 0.0f };

		if (!IsGamepadAvailable(s_settings.gamepad))
			return v;

		float leftStickX = GetGamepadAxisMovement(s_settings.gamepad, GAMEPAD_AXIS_RIGHT_X);
		float leftStickY = GetGamepadAxisMovement(s_settings.gamepad, GAMEPAD_AXIS_RIGHT_Y);
		// Calculate deadzones
		if (leftStickX > -s_settings.rightStickDeadzoneX && leftStickX < s_settings.rightStickDeadzoneX) leftStickX = 0.0f;
		if (leftStickY > -s_settings.rightStickDeadzoneY && leftStickY < s_settings.rightStickDeadzoneY) leftStickY = 0.0f;

		v.x = leftStickX;
		v.y = leftStickY;

		return v;
	}

	bool isConfirmedPressed()
	{
		return isActionPressed("confrim");
	}

	bool isConfirmedDown()
	{
		return isActionDown("confrim");
	}

	bool isConfirmedReleased()
	{
		return isActionReleased("confrim");
	}

	bool isCancelPressed()
	{
		return isActionPressed("cancel");
	}

	bool isCancelDown()
	{
		return isActionDown("cancel");
	}

	bool isCancelReleased()
	{
		return isActionReleased("cancel");
	}

}