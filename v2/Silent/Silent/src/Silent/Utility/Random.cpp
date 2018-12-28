#include "Random.h"

namespace Silent
{
	std::random_device Random::rd;
	std::mt19937 Random::mt(rd());

	int Random::RandomInt(int exclusiveMax)
	{
		std::uniform_int_distribution<> dist(0, exclusiveMax - 1);
		return dist(mt);
	}

	int Random::RandomInt(int min, int max)
	{
		std::uniform_int_distribution<> dist(0, max - min);
		return dist(mt) + min;
	}

	float Random::RandomFloat(float exclusiveMax)
	{
		std::uniform_real_distribution<> dist(0, exclusiveMax - 1.f);
		return (float) dist(mt);
	}

	float Random::RandomFloat(float min, float max)
	{
		std::uniform_real_distribution<> dist(0, max - min);
		return (float) dist(mt) + min;
	}

	bool Random::RandomBool(double probability)
	{
		std::bernoulli_distribution dist(probability);
		return dist(mt);
	}
}