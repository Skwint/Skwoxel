#ifndef SKWOXEL_GEOMETRY_H
#define SKWOXEL_GEOMETRY_H

#include <godot_cpp/variant/vector3.hpp>

namespace skwoxel
{
	inline godot::Vector3 get_closest_point_to_segment(const godot::Vector3& p_point, const godot::Vector3& p_a, const godot::Vector3& p_b)
	{
		godot::Vector3 p = p_point - p_a;
		godot::Vector3 n = p_b - p_a;
		real_t l2 = n.length_squared();
		if (l2 < 1e-20f)
		{
			return p_a; // Both points are the same, just give any.
		}

		real_t d = n.dot(p) / l2;

		if (d <= 0.0f)
		{
			return p_a; // Before first point.
		}
		else if (d >= 1.0f)
		{
			return p_b; // After first point.
		}
		else
		{
			return p_a + n * d; // Inside.
		}
	}
}

#endif