#pragma once
#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <string>
#include <vector>
#include "../UUID.h"

namespace glib
{
	struct Tag {
		std::string name = "Entity";
		bool visible = true, enabled = false;
	};

	struct IDComponent { UUID id = 0x0; };

	struct Transform3D : public Transform {
		Transform3D(Vector3 position = {0.0f, 0.0f, 0.0f})
		{
			translation = position;
			scale = { 1.0f, 1.0f, 1.0f };
			rotation = QuaternionIdentity();
		}


		Matrix toMatrix() const {
			return MatrixMultiply(MatrixMultiply(MatrixScale(scale.x, scale.y, scale.z),
				QuaternionToMatrix(rotation)),
				MatrixTranslate(translation.x, translation.y, translation.z));
		}

		void Inspect(const char* label = "Transform 3D");
	};

	struct Velocity3D : public Vector3 {
		Velocity3D(float x_ = 0, float y_ = 0, float z_ = 0)
		{
			this->x = x_, this->y = y_, this->z = z_;
		}
		Velocity3D(const Velocity3D& v) = default;

		void Inspect(const char* label = "Velocity 3D");
	};

	struct Model3D
	{
		Model* ref = nullptr;
		Color tint = WHITE;


		void Draw(const Transform3D& transform)
		{
			if (ref == nullptr)
				return;

			ref->transform = transform.toMatrix();
			DrawModel(*ref, Vector3Zeros, 1.0f, tint);
		}

		void Inspect(const char* label = "Model 3D");

	};
	

	inline void Transform3D_ApplyVelocity(Transform3D& transform, const Velocity3D& velcoity, float dt)
	{
		transform.translation += velcoity * dt;
	}

}