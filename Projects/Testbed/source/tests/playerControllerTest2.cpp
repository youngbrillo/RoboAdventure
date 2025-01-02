
#include "test.h"
#include <gamelib/gameLib.h>

namespace testbed
{
	struct GameObject
	{
		glib::Transform3D transform;
		glib::Velocity3D velocity;
		//glib::Model3D model;
		Model model;
		Color tint = WHITE;

		GameObject(Vector3 position = { 0.0f, 0.0f, 0.0f }) :
			transform(position)
		{}
		virtual ~GameObject()
		{
			//model.ref = nullptr;
		}

		virtual void Update(float dt, glib::Camera3DController* camera = nullptr)
		{
			glib::Transform3D_ApplyVelocity(transform, velocity, dt);
		}
		virtual void Draw()
		{
			//model.Draw(transform);

			model.transform = transform.toMatrix();
			DrawModel(model, Vector3Zeros, 1.0f, tint);
		}
		virtual void Inspect(const char* label = "GameObject")
		{
			if (ImGui::TreeNode(label))
			{
				transform.Inspect();
				velocity.Inspect();
				//model.Inspect();

				ImGui::TreePop();
			}
		}
	};

	struct GameObject_Animating : public GameObject
	{
		glib::ModelAnimator animator;
		GameObject_Animating(Vector3 position = { 0.0f, 0.0f, 0.0f }) : GameObject(position)
		{}
		~GameObject_Animating()
		{
			animator.UnLoad();
		}

		virtual void Update(float dt, glib::Camera3DController* camera = nullptr) override
		{
			GameObject::Update(dt, camera);
			//animator.Update(dt, *model.ref);
			animator.Update(dt, model);
		}

		virtual void Inspect(const char* label = "Animating GameObject") override
		{
			if (ImGui::TreeNode(label))
			{
				GameObject::Inspect("base");
				animator.inspect();
				ImGui::TreePop();
			}
		}
	};

	struct GameObject_Character : public GameObject_Animating
	{
		float movement_speed = 3.0f;
		bool isMoving = false, inAction = false;
		std::string idleAnim = "idle", moveAnim = "run", actionAnim = "spin";

		Vector3 forward = { 1.0f, 0.0f, 1.0f };
		GameObject_Character(Vector3 position = { 0.0f, 0.0f, 0.0f }) : GameObject_Animating(position)
		{}


		virtual void Update(float dt, glib::Camera3DController* camera = nullptr) override
		{

			if (inAction)
			{
				animator.SetAnimationName(actionAnim.c_str());
			}
			else if (isMoving) 
			{
				animator.SetAnimationName(moveAnim.c_str());

			}
			else 
			{
				animator.SetAnimationName(idleAnim.c_str());
			}



			GameObject_Animating::Update(dt, camera);
		}
		virtual void Inspect(const char* label = "Character GameObject") override
		{
			if (ImGui::TreeNode(label))
			{
				GameObject::Inspect("base");
				animator.inspect();
				ImGui::SliderFloat("movement speed", &movement_speed, 0.5f, 10.0f);
				ImGui::DragFloat3("forward direction", &forward.x);

				ImGui::TreePop();
			}
		}
	};
	struct GameObject_Player : public GameObject_Character
	{
		GameObject_Player(Vector3 position = { 0.0f, 0.0f, 0.0f }) : GameObject_Character(position)
		{}

		virtual void Update(float dt, glib::Camera3DController* camera = nullptr) override
		{

			bool justAttacked = false;
			if (!inAction && glib::io::isCancelReleased())
			{
				inAction = true;
				justAttacked = true;
			}
			if (inAction && !justAttacked)
			{
				//if (glib::io::isCancelReleased())
				if(animator.frameEnded)
					inAction = false;
			}

			if(!inAction)
			{
				Vector2 dir = glib::io::GetLeftJoystickDirection();
				Vector2 inputDir = Vector2Normalize(dir);
				Vector3 moveVec = { inputDir.x * movement_speed, 0.0f, inputDir.y * movement_speed }; //target velocity
				Matrix rotMat = MatrixRotateY(camera->viewAngle.x); // a matrix for the plane rotation
				moveVec = Vector3Transform(moveVec, rotMat); // transform the movement vector into world space, but ignore the tilt so it is in plane



				if (Vector2Length(inputDir) > 0.01f)
				{
					velocity.x = moveVec.x;
					velocity.y = moveVec.y;
					velocity.z = moveVec.z;
					this->isMoving = true;
					Vector2 _dir = { moveVec.x, moveVec.z }; _dir = Vector2Normalize(_dir);
					float angle = atan2f(_dir.x, _dir.y);
					transform.rotation = QuaternionFromAxisAngle(Vector3{ 0.0f, 1.0f, 0.0f }, angle);
				}
				else
				{

					velocity = { 0.0f, 0.0f , 0.0f };
					this->isMoving = false;
				}
			}
			GameObject_Character::Update(dt, camera);
		}

	};

	class PlayerControllerTest2 : public Test
	{
		GameObject_Player player;
		glib::Camera3DController camController;
		std::vector<Model> model_list;
		std::vector<GameObject*> gameObjects;

	public:
		PlayerControllerTest2()
			: Test("Player Controller Test No. 2", GetColor(0x3C443DFF))
		{
			const char* dummyfp = "C:/GameDev/Assets/raildogameart/GLB/dummy.glb";


			Texture2D defaultTexture = { 1,1,1 };

			{
				Mesh mesh = GenMeshCube(1, 1, 1);
				Model& model = model_list.emplace_back(LoadModelFromMesh(mesh));
				model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = defaultTexture;
			}
			{

				Model& model = model_list.emplace_back(LoadModel(dummyfp));
				//player.model.ref = &model;
				player.model = model;
			}
			std::vector<const char*> imports = {
				"C:/GameDev/Assets/raildogameart/GLTF/barrel.gltf"
				, "C:/GameDev/Assets/raildogameart/GLTF/beam.gltf"
				, "C:/GameDev/Assets/raildogameart/GLTF/metal_door.gltf"
				, "C:/GameDev/Assets/raildogameart/GLTF/cube_spike.gltf"
				, "C:/GameDev/Assets/raildogameart/GLTF/wheel_pile.gltf"
				, "C:/GameDev/Assets/raildogameart/GLTF/plat_jump.gltf"
				, "C:/GameDev/Assets/raildogameart/GLTF/metal_sphere.gltf"
				, "C:/GameDev/Assets/raildogameart/GLTF/dumy_stand.gltf"
				, "C:/GameDev/Assets/raildogameart/GLTF/wooden_stand.gltf"
				, "C:/GameDev/Assets/raildogameart/GLTF/plat_spike.gltf"
				//, "C:/GameDev/Assets/raildogameart/GLTF/gridmap.gltf"
				, "C:/GameDev/Assets/raildogameart/GLTF/cone.gltf"
				, "C:/GameDev/Assets/raildogameart/GLTF/plat_move.gltf"
			};
			for (const char* filename : imports)
			{
				if (!FileExists(filename))
					continue;
				Model& model = model_list.emplace_back(LoadModel(filename));
				Vector3 pos = {
					.x = (float)GetRandomValue(-10, 10),
					.y = 0.0f,
					.z = (float)GetRandomValue(-10, 10)
				};
				GameObject* object = gameObjects.emplace_back(new GameObject(pos));
				//object->model.ref = &model;
				object->model = model;
			}

			player.animator.Load(dummyfp);
			player.animator.SetAnimationName("idle");
			player.actionAnim = "roll";
		}

		~PlayerControllerTest2()
		{
			for (auto& model : model_list)
			{
				UnloadModel(model);
			}

			for (auto* obj : gameObjects)
			{
				delete obj;
			}

			gameObjects.clear();
		}

		virtual void Set()
		{
			camController.camera =
			{
				.position = {10.0f, 10.0f, 10.0f},
				.target = {0.0f, 0.0f, 0.0f},
				.up = {0.0f, 1.0f, 0.0f},
				.fovy = 45.0f,
				.projection = CAMERA_PERSPECTIVE
			};

			player.transform = glib::Transform3D();

			camController.target = &player.transform.translation;
		}
		virtual void Unset()
		{

		}

		virtual void Update(float dt)
		{
			camController.Update(dt);
			player.Update(dt, &camController);
			for (auto&& obj : gameObjects) obj->Update(dt, &camController);
		}
		virtual void Draw()
		{
			BeginMode3D(camController.camera);
			DrawGrid(10, 1.0f);
			for (auto&& obj : gameObjects) obj->Draw();
			player.Draw();
			EndMode3D();

			DrawText(name.c_str(), 20, GetScreenHeight() - 40, 20, WHITE);
		};
		virtual void Inspect()
		{
			ImGui::Checkbox("paused", &this->paused);
			ImGui::ColorEditRay("backgroundColor", &this->backgroundColor);
			camController.Inspect();
			player.Inspect("Player");
		};
		static Test* Generate() { return new PlayerControllerTest2(); }
	};

	static int entry = testbed::TestManager::AddEntry("Player Controller Test No. 2", "Cateogry", testbed::PlayerControllerTest2::Generate);
}
