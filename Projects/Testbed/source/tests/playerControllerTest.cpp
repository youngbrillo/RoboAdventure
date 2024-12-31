#include "test.h"

namespace testbed
{
	//Matrix, 2x2 components, column mayor, OpenGL style, right handed.
	typedef struct {
		float m0, m2;
		float m1, m3;
	} DMatrix2x2;

	//Multiply 2x2 DMatrix by Vector2
	Vector2 DVector2MultiplyByMatrix2x2(Vector2 vector, DMatrix2x2 matrix)
	{
		Vector2 result = Vector2{
				matrix.m0 * vector.x + matrix.m1 * vector.y,
				matrix.m2 * vector.x + matrix.m3 * vector.y
		};
		return result;
	}
	struct Player
	{
		Transform playerTransform;
		Model playerModel;

		//Animation
		unsigned int animsCount;
		unsigned int animIndex;
		unsigned int animCurrentFrame;

		ModelAnimation* playerAnimations;
		ModelAnimation currentAnimation;

		//Movement
		float speed;
		float targetHeight;
		bool isMoving;
		float animTimer = 0.0f, animSpeed = 0.016f;

		void Update(Vector2 angle, float dt)
		{
			//Vector rotation angle coordinates. Allows to rotate the inputDirection vector with the camera angle.
			Vector2 vraCoordinates = { 0.0f, 0.0f };
			Vector2 inputDirection = { 0.0f, 0.0f };
			vraCoordinates.x = sinf(angle.x);
			vraCoordinates.y = cosf(angle.x);

			//Bool to check if button is pressed
			this->isMoving = false;

			//Vector rotation angle matrix. Matrix to rotate the x,y of inputDirection. 
			DMatrix2x2 vraMatrix = DMatrix2x2{ vraCoordinates.y, -vraCoordinates.x, vraCoordinates.x, vraCoordinates.y };


			inputDirection.x = IsKeyDown(KEY_A) - IsKeyDown(KEY_D);
			inputDirection.y = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);

			inputDirection = DVector2MultiplyByMatrix2x2(inputDirection, vraMatrix);

			inputDirection = Vector2Normalize(inputDirection);

			//Check wether a button is pressed.
			if (inputDirection.x != 0 || inputDirection.y != 0) {
				this->isMoving = true;
			}

			//Move, rotate and animate player when its moving.
			if (this->isMoving) {
				this->playerTransform.rotation = QuaternionFromAxisAngle(Vector3{ 0.0f, 1.0f, 0.0f }, (atan2f(inputDirection.x, inputDirection.y)));
				this->playerTransform.translation.x += inputDirection.x * dt;
				this->playerTransform.translation.z += inputDirection.y * dt;
				this->animIndex = 6;
			}
			else {
				this->animIndex = 2;
			}

			animTimer += dt;

			this->playerModel.transform = QuaternionToMatrix(this->playerTransform.rotation);
			if (animTimer >= animSpeed) {
				animTimer = 0.0f;
				this->currentAnimation = this->playerAnimations[this->animIndex];
				this->animCurrentFrame = (this->animCurrentFrame + 1) % this->currentAnimation.frameCount;
			}
		}
	};


	struct DCamera {
		Camera3D mainCamera;
		float targetDistance;
		Vector2 angle;
		Vector2 previousMousePosition;

		void Update(Vector3 target, float dt)
		{
			//Register mouse delta and mouse position.
			Vector2 mousePositionDelta = { 0.0f, 0.0f };
			Vector2 mousePosistion = GetMousePosition();

			mousePositionDelta.x = mousePosistion.x - this->previousMousePosition.x;
			mousePositionDelta.y = mousePosistion.y - this->previousMousePosition.y;

			this->previousMousePosition = mousePosistion;

			// Change camera position
			this->mainCamera.target = target;
			this->mainCamera.position.x = sinf(this->angle.x) * this->targetDistance * cosf(this->angle.y) + this->mainCamera.target.x;
			this->mainCamera.position.y = ((this->angle.y <= 0.0f) ? 1 : -1) * sinf(this->angle.y) * this->targetDistance * sinf(this->angle.y) + this->mainCamera.target.y;
			this->mainCamera.position.z = cosf(this->angle.x) * this->targetDistance * cosf(this->angle.y) + this->mainCamera.target.z;

			// Uses mouse to change camera angles
			this->angle.x += (mousePositionDelta.x * -0.005f);
			this->angle.y += (mousePositionDelta.y * -0.005f);

			//Slightly changes angle when A or D are pressed. 
			this->angle.x += (IsKeyDown(KEY_A) - IsKeyDown(KEY_D)) * dt;


			// Clamp y angle
			if (this->angle.y > 89.0f * DEG2RAD)
			{
				this->angle.y = 89.0f * DEG2RAD;
			}
			else if (this->angle.y < 10.0f * DEG2RAD)
			{
				this->angle.y = 10.0f * DEG2RAD;
			}
		}
	};

	// Camera movement/updates


	class PlayerControllerTest : public Test
	{
		Player player1;
		DCamera camera;

	public:
		PlayerControllerTest()
			: Test("Player Controller", LIGHTGRAY)
		{
			const char* modelfp = "C:/GameDev/Projects/repos/raylib/examples/models/resources/models/gltf/robot.glb";

			int animCount = 0;
			player1 = {
				.playerTransform = {0.0f},
				.playerModel = LoadModel(modelfp),
				.animsCount = 0,
				.animIndex = 0,
				.animCurrentFrame = 0,
				.playerAnimations = LoadModelAnimations(modelfp, &animCount)
			};
			player1.animsCount = animCount;

			camera = {
				.mainCamera = Camera3D {
					.position = Vector3{0.0f, 4.0f, 0.0f},
					.target = player1.playerTransform.translation,
					.up = Vector3{0.0f, 1.0f, 0.0f},
					.fovy = 69.0f,
					.projection = CAMERA_PERSPECTIVE
					},
				.targetDistance = -15.0f,
				.angle = 0.0f,
				.previousMousePosition = GetMousePosition()
			};
		}

		~PlayerControllerTest()
		{
			UnloadModel(player1.playerModel);
		}

		virtual void Set()
		{
			player1.playerTransform = { 0.0f, 0.0f, 0.0f };
			camera.mainCamera.position = { 0.0f, 4.0f, 0.0f };
		}
		virtual void Unset()
		{

		}

		virtual void Update(float dt)
		{

			if (IsKeyPressed(KEY_Z))
				if (IsCursorHidden())
					EnableCursor();
				else
					DisableCursor();

			player1.Update(camera.angle, dt);
			camera.Update(player1.playerTransform.translation, dt);
			UpdateModelAnimation(player1.playerModel, player1.currentAnimation, player1.animCurrentFrame);
		}
		virtual void Draw()
		{
			BeginMode3D(camera.mainCamera);
			DrawGrid(10, 1);
			DrawModel(player1.playerModel, player1.playerTransform.translation, 1.0f, WHITE);
			EndMode3D();
			// instructions
			DrawText("Right drag to rotate, Wheel to zoom, WASD to move", 20, GetScreenHeight() - 65, 20, WHITE);
			DrawText(name.c_str(), 20, GetScreenHeight() - 40, 20, WHITE);
		};
		virtual void Inspect()
		{
			ImGui::Checkbox("paused", &this->paused);

		};
		static Test* Generate() { return new PlayerControllerTest(); }
	};

	static int entry = testbed::TestManager::AddEntry("Player Controller", "game", testbed::PlayerControllerTest::Generate);
}