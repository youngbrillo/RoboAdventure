#include "cameras.h"
#include "../../utilities/rl_imgui/edit.h"
#include "../../utilities/io/Input.h"
#include <math.h>

void glib::Camera3DController::Update(float dt)
{
	if (target)
		camera.target = *target;
#if true
	Vector2 dir = io::GetRightJoystickDirection();
	camera.position.x = sinf(this->viewAngle.x) * targetDistance * cosf(viewAngle.y) + camera.target.x;
	camera.position.y = ((this->viewAngle.y <= 0.0f) ? 1 : -1) * sinf(this->viewAngle.y) * this->targetDistance * sinf(this->viewAngle.y) + camera.target.y;
	camera.position.z = cosf(this->viewAngle.x) * this->targetDistance * cosf(this->viewAngle.y) + camera.target.z;

	viewAngle.x += dir.x * pan_speed * dt;
	viewAngle.y += dir.y * pan_speed * dt;

	// Angle clamp
	if (this->viewAngle.y < this->clampMin * DEG2RAD)
		this->viewAngle.y = this->clampMin * DEG2RAD;
	else if (this->viewAngle.y > this->clampMax * DEG2RAD)
		this->viewAngle.y = this->clampMax * DEG2RAD;
#else
	Vector2 delta = io::GetRightJoystickDirection();
	Vector2 moveDir = io::GetLeftJoystickDirection();
	float mouseWheelMove = GetMouseWheelMove();

	float turnRotation = delta.x * turnSpeed.x * dt;
	float tiltRotation = delta.y * turnSpeed.y * dt;

	viewAngle.x -= turnRotation * DEG2RAD;
	viewAngle.y -= tiltRotation * DEG2RAD;

	// Angle clamp
	if (this->viewAngle.y < this->clampMin * DEG2RAD)
		this->viewAngle.y = this->clampMin * DEG2RAD;
	else if (this->viewAngle.y > this->clampMax * DEG2RAD)
		this->viewAngle.y = this->clampMax * DEG2RAD;


	//movement in plane rotation space
	Vector3 moveVec = { 0,0,0 };
	//moveVec.z= moveDir.y * follow_speed * dt;
	//moveVec.x = moveDir.x * follow_speed * dt;
	// update zoom
	targetDistance += mouseWheelMove;
	if (targetDistance < 1)
		targetDistance = 1;

	// vector we are going to transform to get the camera offset from the target point
	Vector3 camPos = { 0, 0, targetDistance };

	Matrix tiltMat = MatrixRotateX(viewAngle.y); // a matrix for the tilt rotation
	Matrix rotMat = MatrixRotateY(viewAngle.x); // a matrix for the plane rotation
	Matrix mat = MatrixMultiply(tiltMat, rotMat); // the combined transformation matrix for the camera position

	camPos = Vector3Transform(camPos, mat); // transform the camera position into a vector in world space
	moveVec = Vector3Transform(moveVec, rotMat); // transform the movement vector into world space, but ignore the tilt so it is in plane

	CameraPosition = Vector3Add(CameraPosition, moveVec); // move the target to the moved position


	camera.target = CameraPosition;
	camera.position = Vector3Add(CameraPosition, camPos);
#endif
}

void glib::Camera3DController::Inspect(const char* label)
{
	if (ImGui::TreeNode(label))
	{
		ImGui::SliderFloat("follow speed", &follow_speed, 1.0f, 10.0f);
		ImGui::SliderFloat("follow pan_speed", &pan_speed, 1.0f, 10.0f);
		ImGui::SliderFloat("target Distance", &targetDistance, 1.0f, 30.0f);
		ImGui::SliderFloat("clamp Min", &clampMin, -180.0f, clampMax);
		ImGui::SliderFloat("clamp Max", &clampMax, clampMin, 180.0f);
		ImGui::InputFloat2("viewAngle", &viewAngle.x);
		ImGui::InputFloat2("_prevMousePosition", &_prevMousePosition.x);
		ImGui::InputFloat2("turnSpeed", &turnSpeed.x);
		ImGui::Camera3DEdit("camera", &camera);

		ImGui::TreePop();
	}
}
