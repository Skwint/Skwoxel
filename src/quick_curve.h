#ifndef SKWOXEL_QUICK_CURVE_H
#define SKWOXEL_QUICK_CURVE_H

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/curve3d.hpp>

#include <vector>

namespace skwoxel
{
	// This is intended to be a faster alternative to the Curve3D baking mechanism
	// but at present it is actually slower (doh) and hence unused.
	// The principle, however, is good. I think it just needs to be recursive instead
	// of using a delta of 0.01 right from the start. 0.1 should be sufficient.
	class QuickCurve
	{
	public:
		struct Point
		{
			godot::Vector3 pos;
			real_t offset;
			real_t distance_squared;
		};
	public:
		QuickCurve();
		void set_curve(godot::Ref<godot::Curve3D> curve);

		Point seek_closest(const godot::Vector3& pos) const;

	protected:
		void seek_closest(const godot::Vector3& pos, Point& point, int bezier_index) const;
		inline godot::Vector3 sample(int index, float offset) const
		{
			return control_points[index].bezier_interpolate(control_points[index + 1], control_points[index + 2], control_points[index + 3], offset);
		}

	protected:
		std::vector<godot::Vector3> control_points;
		Point bad;
	};
}

#endif
