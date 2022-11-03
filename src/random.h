#ifndef SKWOXEL_RANDOM_H
#define SKWOXEL_RANDOM_H

#include <godot_cpp/classes/random_number_generator.hpp>

namespace skwoxel
{
	extern godot::RandomNumberGenerator * rnd;
	extern uint32_t rand();
}

#endif
