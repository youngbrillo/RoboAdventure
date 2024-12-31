
#include "test.h"

namespace testbed
{
	class Template : public Test
	{
		int cameraMode = CAMERA_FIRST_PERSON;
		Camera camera = { 0 };

	public:
		Template()
			: Test("Template", BLACK)
		{
			const char* dummyfp = "C:/GameDev/Assets/raildogameart/GLB/dummy.glb";

		}

		~Template()
		{

		}

		virtual void Set()
		{

		}
		virtual void Unset()
		{

		}

		virtual void Update(float dt)
		{

		}
		virtual void Draw()
		{
			DrawText(name.c_str(), 20, GetScreenHeight() - 40, 20, WHITE);
		};
		virtual void Inspect()
		{
			ImGui::Checkbox("paused", &this->paused);
		};
		static Test* Generate() { return new Template(); }
	};

	static int entry = testbed::TestManager::AddEntry("Template_Name", "Cateogry", testbed::Template::Generate);
}
