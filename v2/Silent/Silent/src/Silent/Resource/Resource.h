#pragma once
#include "../Core.h"

// This class is a base class for all the resources we can load/save in
// our engine
namespace Silent
{
	using ResourceID = unsigned int;

	inline ResourceID GenerateResourceID()
	{
		// ID = 0 would be invalid
		static ResourceID id = 1;
		return id++;
	}

	class SILENT_API Resource
	{
	public:
		ResourceID id = GenerateResourceID();
		const std::string _name;
		bool _initialized;

		Resource(std::string name = "") : _name(name), _initialized(false) {}
		virtual ~Resource() = default;

		virtual inline void Load() = 0;
		virtual inline void Cleanup() = 0;
	};
}