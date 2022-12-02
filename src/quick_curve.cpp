#include "quick_curve.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/geometry3d.hpp>
#include <limits.h>

using namespace godot;

namespace skwoxel
{
	QuickCurve::QuickCurve()
	{
		bad.distance_squared = -1.0;
	}

	void QuickCurve::set_curve(Ref<Curve3D> curve)
	{
		control_points.resize(0);
		if (curve.is_valid())
		{
			int num = curve->get_point_count();
			control_points.reserve(num + 2 * (num - 1));
			for (int pp = 0; pp < num; ++pp)
			{
				Vector3 pos = curve->get_point_position(pp);
				if (pp != 0)
				{
					control_points.push_back(pos + curve->get_point_in(pp));
				}
				control_points.push_back(pos);
				if (pp < num - 1)
				{
					control_points.push_back(pos + curve->get_point_out(pp));
				}
			}
		}
	}

	// Find the closest point on all curves, then select the closest of those
	QuickCurve::Point QuickCurve::seek_closest(const Vector3& pos) const
	{
		if (control_points.empty())
			return bad;

		std::vector<Point> seek_points;
		seek_points.resize((control_points.size() - 1) / 3);

		if (seek_points.empty())
			return bad;

		for (int bezier_index = 0; bezier_index < seek_points.size(); bezier_index++)
		{
			seek_closest(pos, seek_points[bezier_index], bezier_index);
		}

		int closest = 0;
		for (int bezier_index = 1; bezier_index < seek_points.size(); bezier_index++)
		{
			if (seek_points[bezier_index].distance_squared < seek_points[closest].distance_squared)
			{
				closest = bezier_index;
			}
		}

		seek_points[closest].pos = sample(closest * 3, seek_points[closest].offset);
		seek_points[closest].offset = (real_t(closest) + seek_points[closest].offset) / real_t(seek_points.size());
		return seek_points[closest];
	}

	// Find the closest point on just one curve
	void QuickCurve::seek_closest(const Vector3& pos, Point& point, int bezier_index) const
	{
		point.distance_squared = std::numeric_limits<real_t>::max();
		int point_index = bezier_index * 3;
		real_t delta = 0.1;
		real_t from = 0.0;
		real_t to = 1.0;
		for (int iter = 0; iter < 4; ++iter)
		{
			for (real_t t = from; t < to + 0.5 * delta; t += delta)
			{
				Vector3 test = sample(point_index, t);
				real_t distance_squared = (pos - test).length_squared();
				if (distance_squared < point.distance_squared)
				{
					point.distance_squared = distance_squared;
					point.offset = t;
				}
			}
			from = std::max(real_t(0.0), point.offset - delta);
			to = std::min(real_t(1.0), point.offset + delta);
			delta = (to - from) / 9.0;
		}
	}
}
