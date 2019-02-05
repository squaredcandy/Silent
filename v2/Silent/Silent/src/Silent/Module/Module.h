#pragma once

#include "../Core.h"
#include "../Entity/Entity.h"

#include <map>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

namespace Silent
{
	using ModuleID = unsigned int;

	inline ModuleID GenerateModuleTypeID()
	{
		static ModuleID id = 1;
		return id++;
	}

	template <typename T> inline ModuleID GetModuleTypeID()
	{
		static ModuleID typeID = GenerateModuleTypeID();
		return typeID;
	}

	class SILENT_API Entity;

	// Underscore before the variable to indicate that it's a member variable
	// just a convection that im enforcing, more to follow as I work on this
	class SILENT_API Module
	{
	public:
		//// The module type
		//ModuleID _moduleID;
		// The entity that owns this
		std::shared_ptr<Entity> _entity;
		bool _active = true;

	public:
		Module() = default;
		virtual ~Module() = default;

		bool operator==(const Module& other) const
		{
			return _entity == other._entity;
		}
		bool operator<(const Module& other) const
		{
			return _entity < other._entity;
		}
	};

	// So this is combined with the common module cos it better to just have one
	// module that does both rather than two seperate ones
	class SILENT_API MTransform : public Module
	{
		// This is copied over from the common module stuff
		// Entity Name
		std::string _name;
		// Parent ID
		std::shared_ptr<Entity> _parent;

		// Model Vectors
		glm::vec3 _translate;
		glm::vec3 _rotate; // I should store this a quaterion
		glm::vec3 _scale;

		// Direction Vectors
		glm::vec3 _forwardVector;
		glm::vec3 _upVector;
		glm::vec3 _rightVector;

		glm::mat4 _modelMatrix;
		// Check if we need to update the matrix data in the renderer
		bool matrixUpdated;
	public:

		MTransform(std::string name = "", 
						 std::shared_ptr<Entity> parent = nullptr,
						 glm::vec3 translation = glm::vec3(), 
						 glm::vec3 rotation = glm::vec3(),
						 glm::vec3 scale = glm::vec3(1)) :
			_name(name), _parent(parent), 
			_translate(translation), _rotate(rotation), _scale(scale) {}
		virtual ~MTransform() = default;

		friend class ITransform;
	};

	// Camera module
	class SILENT_API MCamera : public Module
	{
	private:
		float fov = 45.f;
		float nearPlane = 0.05f;
		float farPlane = 1000000.f;

	public:
		// Is this the current camera?
		// TODO: probably put this in a better place
		bool currentCamera = false;

		MCamera() = default;
		virtual ~MCamera() = default;

		friend class ICamera;
	};

	// Mesh Module
	//class SILENT_API Resource;
	class SILENT_API RMesh;
	class SILENT_API RMaterial;
	class SILENT_API MModel : public Module
	{
	public:
		std::shared_ptr<RMesh> mesh;
		std::shared_ptr<RMaterial> material;

		MModel() = default;
		virtual ~MModel() = default;
	};
	
	// Render Module
	class SILENT_API RBuffer;
	class SILENT_API MRender : public Module
	{
	protected:
		// render settings?
	public:
		bool render = true;
		std::shared_ptr<RBuffer> buffer;
		MRender() = default;
		virtual ~MRender() = default;
	};

	// This class accepts input from somewhere and 
	// translates it into something
	// That is the plan
	class SILENT_API MInput : public Module
	{
	protected:

	public:
		MInput() = default;
		virtual ~MInput() = default;
	};

	enum LightType
	{
		PointLight			= 0,
		SpotLight			= 1,
		DirectionalLight	= 2
	};

	// This light is a base class for all the lights?
	// - Point Light
	// - Spot Light
	// - Directional Light
	class SILENT_API MLight : public Module
	{
	protected:
	public:
		LightType lightType;
		glm::vec3 lightColor;
		float size;
		float falloffPower;

		MLight() = default;
		virtual ~MLight() = default;
	};
}

//#include <reactphysics3d/reactphysics3d.h>
//namespace rp3d = reactphysics3d;
//namespace Silent
//{
//	class SILENT_API Module_Box_Collider : public Module
//	{
//	protected:
//		rp3d::BoxShape * boxCollider;
//	public:
//		Module_Box_Collider(rp3d::Vector3 v = { 1, 1, 1 }) 
//			: boxCollider(new rp3d::BoxShape(v)) {}
//		virtual ~Module_Box_Collider() { delete boxCollider; }
//	};
//}