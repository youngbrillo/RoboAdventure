#pragma once
#include <raylib/raylib.h>

namespace glib
{
	struct Camera3DController
	{
		float follow_speed = 1.0f;
		float targetDistance = 10.0f;
		Vector2 angle = { 0.0f, 0.0f }, _prevMousePosition = { 0.0f, 0.0f };
		
		Camera3D camera =
		{
			.position = {10.0f, 10.0f, 10.0f},
			.target = {0.0f, 0.0f, 0.0f},
			.up = {0.0f, 1.0f, 0.0f},
			.fovy = 45.0f,
			.projection = CAMERA_PERSPECTIVE
		};

		void Update(float dt);
		void Inspect(const char* label = "Camera 3D Controller");
	};
}