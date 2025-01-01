#pragma once
#include <raylib/raylib.h>

namespace glib::io
{
	enum INPUT_TYPE_
	{
		INPUT_TYPE_KEY = 0,
		INPUT_TYPE_MOUSE_BUTTON,
		INPUT_TYPE_GAMEPAD_BUTTON,
		INPUT_TYPE_GAMEPAD_JOYSTICK
	};
	
	struct InputEvent
	{
		int type = INPUT_TYPE_KEY;
		int value = 0;
	};

	struct InputSettings
	{
		int gamepad = 0;
		//axis deadzones
		float leftStickDeadzoneX = 0.1f;
		float leftStickDeadzoneY = 0.1f;
		float rightStickDeadzoneX = 0.1f;
		float rightStickDeadzoneY = 0.1f;
		float leftTriggerDeadzone = -0.9f;
		float rightTriggerDeadzone = -0.9f;
	};
	void InitInputMap();
	void LoadInputs(const char* inputConfigFile = "./io-inputs.yaml");
	void RegisterInput(const char* alias, int type, int value);
	bool isActionPressed(const char* alias);
	bool isActionDown(const char* alias);
	bool isActionReleased(const char* alias);

	Vector2 GetMoveDirection();
	Vector2 GetLeftJoystickDirection();
	Vector2 GetRightJoystickDirection();


	bool isConfirmedPressed();
	bool isConfirmedDown();
	bool isConfirmedReleased();

	bool isCancelPressed();
	bool isCancelDown();
	bool isCancelReleased();
}