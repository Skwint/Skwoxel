#ifndef SKWOXEL_QUICK_CURVE_H
#define SKWOXEL_QUICK_CURVE_H

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/curve3d.hpp>

#include "lotsa.h"

namespace skwoxel
{
	// This is intended to be a faster alternative to the Curve3D baking mechanism
	// but at present it is actually slower (doh) and hence unused.
	// The principle, however, is good. I think it just needs to be recursive instead
	// of using a delta of 0.01 right from the start. 0.1 should be sufficient.
	class QuickCurve
	{
	public:
		QuickCurve();
		void set_curve(godot::Ref<godot::Curve3D> curve);

		bool seek_closest(const godot::Vector3& pos) const;
		const godot::Vector3& get_seek_point() const { return seek_point; }
		int get_seek_index() const { return seek_index; }
		real_t get_seek_offset() const { return seek_offset; }
		real_t get_seek_distance_squared() const { return seek_distance_squared; }
		real_t get_seek_total_offset() const;

	protected:
		inline godot::Vector3 sample(int index, float offset) const
		{
			return points[index].bezier_interpolate(points[index + 1], points[index + 2], points[index + 3], offset);
		}

	protected:
		lotsa<godot::Vector3> points;
		mutable godot::Vector3 seek_point;
		mutable godot::Vector3 seek_before;
		mutable godot::Vector3 seek_after;
		mutable int seek_index;
		mutable real_t seek_offset;
		mutable real_t seek_distance_squared;
	};
}

#endif
