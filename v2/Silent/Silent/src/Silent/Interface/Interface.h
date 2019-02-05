#pragma once
#include "../Core.h"
#include "../Module/Module.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Silent
{
	class ITransform
	{
	private:
		template<typename T> static inline void UpdateModelMatrix(T tf)
		{
			const glm::vec3 xRot{ 1, 0, 0 };
			const glm::vec3 yRot{ 0, 1, 0 };
			const glm::vec3 zRot{ 0, 0, 1 };

			// Translate -> Scale -> Rotate
			tf->_modelMatrix = glm::translate(glm::mat4(), tf->_translate);
			tf->_modelMatrix = glm::scale(tf->_modelMatrix, tf->_scale);
			tf->_modelMatrix = glm::rotate(tf->_modelMatrix, glm::radians(tf->_rotate.x), xRot);
			tf->_modelMatrix = glm::rotate(tf->_modelMatrix, glm::radians(tf->_rotate.y), yRot);
			tf->_modelMatrix = glm::rotate(tf->_modelMatrix, glm::radians(tf->_rotate.z), zRot);
			tf->matrixUpdated = true;
		}
	public:
		template<typename T> static inline void UpdateRotationVectors(T tf)
		{
			auto yawRad = glm::radians(tf->_rotate[0]);
			auto pitchRad = glm::radians(tf->_rotate[1]);
			auto cYawRad = cosf(yawRad);
			auto cPitchRad = cosf(pitchRad);
			auto sYawRad = sinf(yawRad);
			auto sPitchRad = sinf(pitchRad);

			const glm::vec3 worldUpVector{ 0.f, 1.f, 0.f };

			auto forward = glm::vec3(cYawRad * cPitchRad, sPitchRad,
									 sYawRad * cPitchRad);
			tf->_forwardVector = glm::normalize(forward);
			tf->_rightVector = glm::normalize(
				glm::cross(tf->_forwardVector, worldUpVector));
			tf->_upVector = glm::normalize(
				glm::cross(tf->_rightVector, tf->_forwardVector));
		}
		template<typename T> static inline void Translate(T tf, glm::vec3 t)
		{
			tf->_translate = t;
			UpdateModelMatrix(tf);
		}
		template<typename T> static inline void Rotate(T tf, glm::vec3 r)
		{
			tf->_rotate = r;
			UpdateRotationVectors(tf);
			UpdateModelMatrix(tf);
		}
		template<typename T> static inline void Scale(T tf, glm::vec3 s)
		{
			tf->_scale = s;
			UpdateModelMatrix(tf);
		}
		template<typename T> static inline void DebugTRS(T tf)
		{
			if (ImGui::DragFloat3("Translate", &tf->_translate[0], 0.1f))
			{
				UpdateModelMatrix(tf);
			}
			if(ImGui::DragFloat3("Rotate", &tf->_rotate[0], 0.1f))
			{
				UpdateModelMatrix(tf);
			}
			if (ImGui::DragFloat3("Scale", &tf->_scale[0], 0.1f))
			{
				UpdateModelMatrix(tf);
			}
		}
		template<typename T> static inline const std::string& Name(T tf)
		{
			return tf->_name;
		}
		template<typename T> static inline const glm::vec3& Translation(T tf)
		{
			return tf->_translate;
		}
		template<typename T> static inline const glm::vec3& Rotation(T tf)
		{
			return tf->_rotate;
		}
		template<typename T> static inline const glm::vec3& Scale(T tf)
		{
			return tf->_scale;
		}
		template<typename T> static inline const glm::vec3& ForwardVector(T tf)
		{
			return tf->_forwardVector;
		}
		template<typename T> static inline const glm::vec3& RightVector(T tf)
		{
			return tf->_rightVector;
		}
		template<typename T> static inline const glm::vec3& UpVector(T tf)
		{
			return tf->_upVector;
		}
		template<typename T> static inline const glm::mat4  ViewMatrix(T tf)
		{
			return glm::lookAt(tf->_translate,
							   tf->_translate + tf->_forwardVector,
							   tf->_upVector);
		}
		template<typename T> static inline const glm::mat4& ModelMatrix(T tf)
		{
			return tf->_modelMatrix;
		}
		template<typename T> static inline const bool       MatrixUpdated(T tf)
		{
			return tf->matrixUpdated;
		}
		template<typename T> static inline const glm::vec3  FaceCameraVector(T tf)
		{
			return { 0, tf._rotate.x + 90.f, 0 };
		}
	};

	class ICamera
	{
	private:
	public:
		template<typename T> static inline       float&     Fov(T c)
		{
			return c->fov;
		}
		template<typename T> static inline       float&     NearPlane(T c)
		{
			return c->nearPlane;
		}
		template<typename T> static inline       float&     FarPlane(T c)
		{
			return c->farPlane;
		}
		template<typename T> static inline const glm::mat4  ProjectionMatrix(T c, ImVec2& size)
		{
			return glm::perspective(c->fov, size.x / size.y, c->nearPlane, c->farPlane);
		}
	};
}