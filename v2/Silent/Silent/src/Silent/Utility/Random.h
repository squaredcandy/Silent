#pragma once

#include "../Core.h"
#include <FastNoise.h>
#include <random>

namespace Silent
{
	class SILENT_API Random
	{
	private:
		static std::random_device rd;
		static std::mt19937 mt;
		static FastNoise noise;
	public:
		static int RandomInt(int exclusiveMax = 2);
		static int RandomInt(int min, int max); // inclusive min/max
		static float RandomFloat(float exclusiveMax = 2.f);
		static float RandomFloat(float min, float max); // inclusive min/max
		static bool RandomBool(double probability = 0.5);
		static float PerlinNoise(float x, float y);
		static float PerlinNoise(float x, float y, float z);
		static float SimplexNoise(float x, float y);
		static float SimplexNoise(float x, float y, float z);
		static float SimplexNoise(float x, float y, float z, float w);
	};
}