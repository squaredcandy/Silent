#pragma once

#ifdef SL_PLATFORM_WINDOWS
	#ifdef SL_BUILD_DLL
		#define SILENT_API __declspec(dllexport)
	#else
		#define SILENT_API __declspec(dllimport)
	#endif
#else
	#error Silent only supports windows!
#endif

#pragma warning(disable: 4251)

// This is where we define the folders
#define DATA_FOLDER "Data/"
#define MESH_FOLDER DATA_FOLDER "Meshes/"
#define TEXTURE_FOLDER DATA_FOLDER "Textures/"

// This is where we define the engine returns (WIP)
// Errors
#define SL_ERROR_NONE			0x00000000
#define SL_ERROR_INIT			0x00010001

// Notifies
#define SL_NOTIFY_NONE			0x00020000
#define SL_NOTIFY_QUIT			0x00020001

// 0x0003 reserved for platforms
// 0x0004 reserved for renderers