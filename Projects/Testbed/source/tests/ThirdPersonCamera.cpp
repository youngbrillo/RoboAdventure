
#include "test.h"
#include <gamelib/gameLib.h>
#include <gamelib/utilities/rl_imgui/rlTPCamera.h>

namespace testbed
{
	class ThirdPersonControllerTest : public Test
	{
		rlTPCamera orbitCam;
		Mesh mesh;
		Model model;
		Texture2D texture;
	public:
		ThirdPersonControllerTest()
			: Test("3rd Person Controller", BLACK)
		{
			mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
			model = LoadModelFromMesh(mesh);
			texture = LoadTextureFromImage(GenImageChecked(32, 32, 16, 16, WHITE, BLACK));
			model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;
		}

		~ThirdPersonControllerTest()
		{
			UnloadTexture(texture);
			UnloadModel(model);
		}

		virtual void Set()
		{
			rlTPCameraInit(&orbitCam, 45, Vector3{ 1, 0, 0 });
			orbitCam.ViewAngles.y = -15 * DEG2RAD;
		}
		virtual void Unset()
		{

		}

		virtual void Update(float dt)
		{
			rlTPCameraUpdate(&orbitCam);

		}
		virtual void Draw()
		{
			rlTPCameraBeginMode3D(&orbitCam);
			DrawGrid(20, 1.0f);

			// grid of cubes on a plane to make a "world"
			DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BLUE); // simple world plane
			float spacing = 3;
			int count = 5;

			for (float x = -count * spacing; x <= count * spacing; x += spacing)
			{
				for (float z = -count * spacing; z <= count * spacing; z += spacing)
				{
					DrawMesh(mesh, model.materials[0], MatrixTranslate(x, 0.5f, z));
				}
			}

			// target point
			DrawSphere(orbitCam.CameraPosition, 0.25f, RED);

			rlTPCameraEndMode3D();

			DrawText(name.c_str(), 20, GetScreenHeight() - 40, 20, WHITE);
		};
		virtual void Inspect()
		{
			ImGui::Checkbox("paused", &this->paused);
            ImGui::ColorEditRay("backgroundColor", &this->backgroundColor);
		};
		static Test* Generate() { return new ThirdPersonControllerTest(); }
	};

	static int entry = testbed::TestManager::AddEntry("3rd Person Controller ", "3d", testbed::ThirdPersonControllerTest::Generate);
}
