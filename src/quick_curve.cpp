#include "quick_curve.h"

#include <godot_cpp/classes/geometry3d.hpp>

using namespace godot;

namespace skwoxel
{

	static const Vector3 dummy_point(999999999.0, 999999999.0, 999999999.0);

	QuickCurve::QuickCurve()
	{

	}

	void QuickCurve::set_curve(Ref<Curve3D> curve)
	{
		points.resize(0);
		if (curve.is_valid())
		{
			int num = curve->get_point_count();
			points.reserve(num + 2 * (num - 1));
			for (int pp = 0; pp < num; ++pp)
			{
				Vector3 pos = curve->get_point_position(pp);
				if (pp != 0)
				{
					points.push_back(pos + curve->get_point_in(pp));
				}
				points.push_back(pos);
				if (pp < num - 1)
				{
					points.push_back(pos + curve->get_point_out(pp));
				}
			}
		}
	}

	bool QuickCurve::seek_closest(const Vector3& pos) const
	{
		if (points.size() == 0)
		{
			return false;
		}

		bool no_before = false;
		bool no_after = false;
		Vector3 nearest_point = points[0];
		int nearest_index = 0;
		real_t nearest_offset = 0.0;
		real_t min_distance_squared = 999999999.0;;
		const real_t delta = 0.01;

		// Find the closest of a point set quickly and cheaply
		for (int pp = 0; pp < points.size() - 3; pp += 3)
		{
			for (real_t tt = 0.0; tt < 1.0 + 0.5 * delta; tt += delta)
			{
				Vector3 point = sample(pp, tt);
				real_t distance_squared = (pos - point).length_squared();
				if (distance_squared < min_distance_squared)
				{
					nearest_point = point;
					nearest_index = pp;
					nearest_offset = tt;
					min_distance_squared = distance_squared;
				}
			}
		}
		// Approximate the segments before and after the point to lines
		// and find a more accurate answer
		int just_before_index;
		int just_after_index;
		real_t just_before_offset;
		real_t just_after_offset;
		if (nearest_offset < 0.5 * delta)
		{
			if (nearest_index > 2)
			{
				just_before_index = nearest_index - 3;
				just_before_offset = 1.0 - delta;
			}
			else
			{
				no_before = true;
				just_before_index = 0;
				just_before_offset = 0.0;
			}
		}
		else
		{
			just_before_index = nearest_index;
			just_before_offset = nearest_offset - delta;
		}
		if (nearest_offset > 1.0 - 0.5 * delta)
		{
			if (nearest_index < points.size() - 3)
			{
				just_after_index = nearest_index + 3;
				just_after_offset = delta;
			}
			else
			{
				no_after = true;
				just_after_index = nearest_index;
				just_after_offset = 1.0;
			}
		}
		else
		{
			just_after_index = nearest_index;
			just_after_offset = nearest_offset + delta;
		}
		seek_before = sample(just_before_index, just_before_offset);
		seek_after = sample(just_after_index, just_after_offset);
		Vector3 close_before;
		Vector3 close_after;
		if (no_before)
			close_before = seek_before;
		else
			close_before = Geometry3D::get_singleton()->get_closest_point_to_segment(pos, seek_before, nearest_point);
		if (no_after)
			close_after = seek_after;
		else		 
			close_after = Geometry3D::get_singleton()->get_closest_point_to_segment(pos, nearest_point, seek_after);

		// Choose whichever result is closest
		float distance_before_squared = (pos - close_before).length_squared();
		float distance_after_squared = (pos - close_after).length_squared();
		if (distance_before_squared < distance_after_squared)
		{
			seek_point = close_before;
			seek_after = nearest_point;
			seek_distance_squared = distance_before_squared;
		}
		else
		{
			seek_point = close_after;
			seek_before = nearest_point;
			seek_distance_squared = distance_after_squared;
		}
		seek_index = nearest_index;
		seek_offset = nearest_offset;
		return true;
	}

	real_t QuickCurve::get_seek_total_offset() const
	{
		/*
		Vector3 dir = (seek_after - seek_before);
		float length_squared = dir.length_squared();
		float dist = (seek_point - seek_before).dot(dir) / length_squared;
		*/
		return (real_t(seek_index / 3) + seek_offset) / real_t((points.size() - 1) / 3);
	}
}
