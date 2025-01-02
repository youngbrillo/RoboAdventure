#pragma once
#include <raylib/raylib.h>
#include <raylib/raymath.h>

namespace glib
{
	struct Camera3DController
	{
		float follow_speed = 1.0f, pan_speed = 3.5f;
		float targetDistance = 10.0f;
		Vector2 
			_prevMousePosition = { 0.0f, 0.0f }
			, turnSpeed = {90, 90}
			, viewAngle = {0.0f, 0.0f}
		;
		float clampMin = -89.0f, clampMax = -10.0f;
		
		Vector3 CameraPosition = { 10.0f, 10.0f, 10.0f };
		Vector3* target = nullptr;
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