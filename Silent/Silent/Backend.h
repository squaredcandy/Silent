#pragma once

#include "Settings.h"

#include <SDL/SDL.h>

#include <string>
#include <iostream>

enum Keycode
{
	KEYCODE_A = SDL_SCANCODE_A,
	KEYCODE_B = SDL_SCANCODE_B,
	KEYCODE_C = SDL_SCANCODE_C,
	KEYCODE_D = SDL_SCANCODE_D,
	KEYCODE_E = SDL_SCANCODE_E,
	KEYCODE_F = SDL_SCANCODE_F,
	KEYCODE_G = SDL_SCANCODE_G,
	KEYCODE_H = SDL_SCANCODE_H,
	KEYCODE_I = SDL_SCANCODE_I,
	KEYCODE_J = SDL_SCANCODE_J,
	KEYCODE_K = SDL_SCANCODE_K,
	KEYCODE_L = SDL_SCANCODE_L,
	KEYCODE_M = SDL_SCANCODE_M,
	KEYCODE_N = SDL_SCANCODE_N,
	KEYCODE_O = SDL_SCANCODE_O,
	KEYCODE_P = SDL_SCANCODE_P,
	KEYCODE_Q = SDL_SCANCODE_Q,
	KEYCODE_R = SDL_SCANCODE_R,
	KEYCODE_S = SDL_SCANCODE_S,
	KEYCODE_T = SDL_SCANCODE_T,
	KEYCODE_U = SDL_SCANCODE_U,
	KEYCODE_V = SDL_SCANCODE_V,
	KEYCODE_W = SDL_SCANCODE_W,
	KEYCODE_X = SDL_SCANCODE_X,
	KEYCODE_Y = SDL_SCANCODE_Y,
	KEYCODE_Z = SDL_SCANCODE_Z,

	KEYCODE_1 = SDL_SCANCODE_1,
	KEYCODE_2 = SDL_SCANCODE_2,
	KEYCODE_3 = SDL_SCANCODE_3,
	KEYCODE_4 = SDL_SCANCODE_4,
	KEYCODE_5 = SDL_SCANCODE_5,
	KEYCODE_6 = SDL_SCANCODE_6,
	KEYCODE_7 = SDL_SCANCODE_7,
	KEYCODE_8 = SDL_SCANCODE_8,
	KEYCODE_9 = SDL_SCANCODE_9,
	KEYCODE_0 = SDL_SCANCODE_0,
	KEYCODE_LEN = KEYCODE_0 + 1
};

class Backend
{
public:
	Backend();
	virtual ~Backend();

	virtual void Init() = 0;
	virtual void SettingsInit() = 0;
	virtual bool UpdateEvent() = 0;
	virtual void Update() = 0;
	virtual void Clear() = 0;
	virtual void Render() = 0;
	virtual void Cleanup() = 0;

	//
	virtual void SetVSync(bool on) = 0;
	virtual void ResizeWindow(int w, int h) = 0;
};