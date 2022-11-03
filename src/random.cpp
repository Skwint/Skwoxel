#include "random.h"

namespace skwoxel
{
	godot::RandomNumberGenerator * rnd = 0;

	uint32_t rand()
	{
		if (!rnd)
		{
			rnd = new godot::RandomNumberGenerator();
			rnd->randomize();
		}
		return rnd->randi();
	}
}

