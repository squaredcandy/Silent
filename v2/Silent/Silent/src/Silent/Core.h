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

#define BIND_1(x) std::bind(&x, this, std::placeholders::_1)
#define BIND_2(x) std::bind(&x, this, std::placeholders::_1, std::placeholders::_2)

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
#define SL_PLATFORM_NONE		0x00030000
#define SL_PLATFORM_SDL			0x00030001

// 0x0004 reserved for renderers
#define SL_RENDERER_NONE		0x00040000
#define SL_RENDERER_GL3			0x00040001
#define SL_RENDERER_VLK			0x00040002

// #define SL_ASSERT_ENABLED
// #ifdef SL_ASSERT_ENABLED
// 	#define SL_ASSERT(x, ...) { if(!(x)) {SL_ERROR("Assertion Failed")}}
// #else
// 
// #endif

// Our own key codes
typedef enum
{
	SL_KEY_A = 4,
	SL_KEY_B = 5,
	SL_KEY_C = 6,
	SL_KEY_D = 7,
	SL_KEY_E = 8,
	SL_KEY_F = 9,
	SL_KEY_G = 10,
	SL_KEY_H = 11,
	SL_KEY_I = 12,
	SL_KEY_J = 13,
	SL_KEY_K = 14,
	SL_KEY_L = 15,
	SL_KEY_M = 16,
	SL_KEY_N = 17,
	SL_KEY_O = 18,
	SL_KEY_P = 19,
	SL_KEY_Q = 20,
	SL_KEY_R = 21,
	SL_KEY_S = 22,
	SL_KEY_T = 23,
	SL_KEY_U = 24,
	SL_KEY_V = 25,
	SL_KEY_W = 26,
	SL_KEY_X = 27,
	SL_KEY_Y = 28,
	SL_KEY_Z = 29,

	SL_KEY_1 = 30,
	SL_KEY_2 = 31,
	SL_KEY_3 = 32,
	SL_KEY_4 = 33,
	SL_KEY_5 = 34,
	SL_KEY_6 = 35,
	SL_KEY_7 = 36,
	SL_KEY_8 = 37,
	SL_KEY_9 = 38,
	SL_KEY_0 = 39,

	SL_KEY_RETURN = 40,
	SL_KEY_ESCAPE = 41,
	SL_KEY_BACKSPACE = 42,
	SL_KEY_TAB = 43,
	SL_KEY_SPACE = 44,

	SL_KEY_MINUS = 45,
	SL_KEY_EQUALS = 46,
	SL_KEY_LEFTBRACKET = 47,
	SL_KEY_RIGHTBRACKET = 48,
	SL_KEY_BACKSLASH = 49,
	SL_KEY_NONUSHASH = 50,
	SL_KEY_SEMICOLON = 51,
	SL_KEY_APOSTROPHE = 52,
	SL_KEY_GRAVE = 53,
	SL_KEY_COMMA = 54,
	SL_KEY_PERIOD = 55,
	SL_KEY_SLASH = 56,

	SL_KEY_CAPSLOCK = 57,

	SL_KEY_F1 = 58,
	SL_KEY_F2 = 59,
	SL_KEY_F3 = 60,
	SL_KEY_F4 = 61,
	SL_KEY_F5 = 62,
	SL_KEY_F6 = 63,
	SL_KEY_F7 = 64,
	SL_KEY_F8 = 65,
	SL_KEY_F9 = 66,
	SL_KEY_F10 = 67,
	SL_KEY_F11 = 68,
	SL_KEY_F12 = 69,
	
	SL_KEY_PRINTSCREEN = 70,
	SL_KEY_SCROLLLOCK = 71,
	SL_KEY_PAUSE = 72,
	SL_KEY_INSERT = 73,
	SL_KEY_HOME = 74,
	SL_KEY_PAGEUP = 75,
	SL_KEY_DELETE = 76,
	SL_KEY_END = 77,
	SL_KEY_PAGEDOWN = 78,
	SL_KEY_RIGHT = 79,
	SL_KEY_LEFT = 80,
	SL_KEY_DOWN = 81,
	SL_KEY_UP = 82,
	
	SL_KEY_NUMLOCKCLEAR = 83,
	SL_KEY_KP_DIVIDE = 84,
	SL_KEY_KP_MULTIPLY = 85,
	SL_KEY_KP_MINUS = 86,
	SL_KEY_KP_PLUS = 87,
	SL_KEY_KP_ENTER = 88,
	SL_KEY_KP_1 = 89,
	SL_KEY_KP_2 = 90,
	SL_KEY_KP_3 = 91,
	SL_KEY_KP_4 = 92,
	SL_KEY_KP_5 = 93,
	SL_KEY_KP_6 = 94,
	SL_KEY_KP_7 = 95,
	SL_KEY_KP_8 = 96,
	SL_KEY_KP_9 = 97,
	SL_KEY_KP_0 = 98,
	SL_KEY_KP_PERIOD = 99,

	SL_NUM_SCANCODES = 512
} SL_Key;