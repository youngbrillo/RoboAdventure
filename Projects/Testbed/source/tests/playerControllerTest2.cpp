
#include "test.h"
#include <gamelib/gameLib.h>

namespace testbed
{
	struct GameObject
	{
		glib::Transform3D transform;
		glib::Velocity3D velocity;
		glib::Model3D model;


		GameObject(Vector3 position = { 0.0f, 0.0f, 0.0f }) :
			transform(position)
		{}


		virtual void Update(float dt)
		{
			glib::Transform3D_ApplyVelocity(transform, velocity, dt);
		}
		virtual void Draw()
		{
			model.Draw(transform);
		}
		virtual void Inspect(const char* label = "GameObject")
		{
			if (ImGui::TreeNode(label))
			{
				transform.Inspect();
				velocity.Inspect();
				model.Inspect();

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

		virtual void Update(float dt) override
		{
			GameObject::Update(dt);
			animator.Update(dt, *model.ref);
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

	class PlayerControllerTest2 : public Test
	{
		int cameraMode = CAMERA_FIRST_PERSON;
		GameObject_Animating player;
		Camera3D camera;

		std::vector<Model> model_list;

	public:
		PlayerControllerTest2()
			: Test("Player Controller Test No. 2", BLACK)
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
				player.model.ref = &model;
			}

			player.animator.Load(dummyfp);
			player.animator.SetAnimationName("idle");
		}

		~PlayerControllerTest2()
		{
			for (auto& model : model_list)
			{
				UnloadModel(model);
			}
		}

		virtual void Set()
		{
			camera =
			{
				.position = {10.0f, 10.0f, 10.0f},
				.target = {0.0f, 0.0f, 0.0f},
				.up = {0.0f, 1.0f, 0.0f},
				.fovy = 45.0f,
				.projection = CAMERA_PERSPECTIVE
			};

			player.transform = glib::Transform3D();
		}
		virtual void Unset()
		{

		}

		virtual void Update(float dt)
		{
			player.Update(dt);
		}
		virtual void Draw()
		{
			BeginMode3D(camera);
			DrawGrid(10, 1.0f);
			player.Draw();
			EndMode3D();

			DrawText(name.c_str(), 20, GetScreenHeight() - 40, 20, WHITE);
		};
		virtual void Inspect()
		{
			ImGui::Checkbox("paused", &this->paused);
			ImGui::Camera3DEdit("camera", &this->camera);
			player.Inspect("Player");
		};
		static Test* Generate() { return new PlayerControllerTest2(); }
	};

	static int entry = testbed::TestManager::AddEntry("Player Controller Test No. 2", "Cateogry", testbed::PlayerControllerTest2::Generate);
}
