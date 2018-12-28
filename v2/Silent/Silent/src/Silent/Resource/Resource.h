#pragma once
#include "../Core.h"

// This class is a base class for all the resources we can load/save in
// our engine
namespace Silent
{
	// These are just points wrapped up in a class so we can safely pass it 
	// around. It also abstracts the functionality of the pointers
	class SILENT_API Resource
	{
	public:
		bool _initialized;

		Resource() : _initialized(false) {}
		virtual ~Resource() = default;

		virtual inline void Cleanup() = 0;
	};
}