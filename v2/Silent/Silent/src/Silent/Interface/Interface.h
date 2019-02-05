#pragma once
#include "../Core.h"
#include "../Module/Module.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Silent
{
	class ITransform
	{
	public:
		static inline void Translate(MTransform * tf, glm::vec3 t)
		{
			tf->_translate = t;
			tf->updateMatrix = true;
		}
		static inline void Rotate(MTransform * tf, glm::vec3 r)
		{
			tf->_rotate = r;
			tf->updateMatrix = true;
		}
		static inline void Scale(MTransform * tf, glm::vec3 s)
		{
			tf->_scale = s;
			tf->updateMatrix = true;
		}
		static inline const glm::vec3& GetTranslate(MTransform * tf)
		{
			tf->updateMatrix = true;
			return tf->_translate;
		}
		static inline const glm::vec3& GetRotate(MTransform * tf)
		{
			tf->updateMatrix = true;
			return tf->_rotate;
		}
		static inline const glm::vec3& GetScale(MTransform * tf)
		{
			tf->updateMatrix = true;
			return tf->_scale;
		}
		static inline const glm::mat4& UpdateModelMatrix(MTransform * tf)
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
			tf->updateMatrix = false;
			return tf->_modelMatrix;
		}
	};
}