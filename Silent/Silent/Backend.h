#pragma once

#include "Settings.h"

#include <SDL/SDL.h>

#include <string>
#include <iostream>

class Backend
{
public:
	Backend();
	virtual ~Backend();

	virtual void Init(SDL_Window *& window) = 0;
	virtual void SettingsInit() = 0;
	virtual void Update() = 0;
	virtual void Clear() = 0;
	virtual void Render(SDL_Window * window) = 0;
	virtual void Cleanup() = 0;
};