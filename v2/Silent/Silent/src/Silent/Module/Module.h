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
// 		Module(const Module &) = delete;
// 		Module(Module &&) = default;
// 		Module& operator=(const Module&) = delete;
// 		Module& operator=(Module&&) = default;

		//constexpr bool operator<(const Module& other) const
		//{
		//	return (_entityID < other._entityID);
		//}
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
	class SILENT_API Module_Transform : public Module
	{
	public:
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
		bool updateMatrix;

		Module_Transform(std::string name = "", 
						 std::shared_ptr<Entity> parent = nullptr,
						 glm::vec3 translation = glm::vec3(), 
						 glm::vec3 rotation = glm::vec3(),
						 glm::vec3 scale = glm::vec3(1)) :
			_name(name), _parent(parent), 
			_translate(translation), _rotate(rotation), _scale(scale) {}
		virtual ~Module_Transform() = default;
	};

	// Camera module
	class SILENT_API Module_Camera : public Module
	{
	public:
		float fov = 45.f;
		float nearPlane = 0.05f;
		float farPlane = 1000000.f;

		// Distance from the camera in 
		// which we don't update the light
		float lightDistance = 100.f;

		// Move this to somewhere else
		float translateSpeed = 5.f;
		float rotateSpeed = 10.f;

	public:
		// Is this the current camera?
		// TODO: probably put this in a better place
		bool currentCamera = false;

		Module_Camera() = default;
		virtual ~Module_Camera() = default;
	};

	// Mesh Module
	//class SILENT_API Resource;
	class SILENT_API Resource_Mesh;
	class SILENT_API Resource_Material;
	class SILENT_API Module_Model : public Module
	{
	public:
		std::shared_ptr<Resource_Mesh> mesh;
		std::shared_ptr<Resource_Material> material;

		Module_Model() = default;
		virtual ~Module_Model() = default;
	};
	
	// Render Module
	class SILENT_API Resource_Buffer;
	class SILENT_API Module_Render : public Module
	{
	protected:
		// render settings?
	public:
		bool render = true;
		std::shared_ptr<Resource_Buffer> buffer;
		Module_Render() = default;
		virtual ~Module_Render() = default;
	};

	// This class accepts input from somewhere and 
	// translates it into something
	// That is the plan
	class SILENT_API Module_Input : public Module
	{
	protected:

	public:
		Module_Input() = default;
		virtual ~Module_Input() = default;
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
	class SILENT_API Module_Light : public Module
	{
	protected:
	public:
		LightType lightType;
		glm::vec3 lightColor;
		float size;
		float falloffPower;

		Module_Light() = default;
		virtual ~Module_Light() = default;
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