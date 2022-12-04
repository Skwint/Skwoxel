#include "skwoxel_field_twist.h"

#include <algorithm>
#include <godot_cpp/classes/geometry3d.hpp>
#include <godot_cpp/variant/Vector2.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "geometry.h"
#include "skwoxel_helpers.h"

using namespace godot;
using namespace std;

namespace skwoxel
{
	void SkwoxelFieldTwist::_notification(int p_what) {
	}

	bool SkwoxelFieldTwist::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(num_points);
		SKWOXEL_SET_METHOD(length);

		return false;
	}

	bool SkwoxelFieldTwist::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(num_points);
		SKWOXEL_GET_METHOD(length);

		return false;
	}

	String SkwoxelFieldTwist::_to_string() const {
		return "[Skwoxel field : TWIST]";
	}

	void SkwoxelFieldTwist::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldTwist::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldTwist::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldTwist::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwist, num_points);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwist, length);
		ClassDB::bind_method(D_METHOD("get_point", "index"), &SkwoxelFieldTwist::get_point, Variant::INT);
		ClassDB::bind_method(D_METHOD("set_point", "index", "pos"), &SkwoxelFieldTwist::set_point, Variant::INT, Variant::VECTOR2);

		// Properties
		SKWOXEL_ADD_PROPERTY(Variant::INT, num_points);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, length);
	}

	SkwoxelFieldTwist::SkwoxelFieldTwist() :
		SkwoxelFieldAdd(),
		length(10.0)
	{
		set_num_points(2);
	}

	SkwoxelFieldTwist::~SkwoxelFieldTwist()
	{

	}

	real_t SkwoxelFieldTwist::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		if (pos.z < 0.0 || pos.z > length)
			return SkwoxelFieldAdd::strength(pos, untransformed, thread_num);
		
		real_t hop_length = length / (points.size() - 1);
		real_t ctrl_step = hop_length * 0.4;
		real_t idx_f = (pos.z / length) * real_t(points.size() - 1);
		int idx = min(points.size() - 2, size_t(idx_f));
		Vector3 from(points[idx].x, points[idx].y, hop_length * idx);
		Vector3 out = from;
		out.z += ctrl_step;
		Vector3 to(points[idx + 1].x, points[idx + 1].y, hop_length * (idx + 1));
		Vector3 in = to;
		in.z -= ctrl_step;
		real_t t = pos.z - hop_length * real_t(idx);
		t /= hop_length;
		Vector3 offset = from.bezier_interpolate(out, in, to, t);
		return SkwoxelFieldAdd::strength(Vector3(offset.x + pos.x, offset.y + pos.y, pos.z), untransformed, thread_num);
	}

	void SkwoxelFieldTwist::set_num_points(int p_num_points)
	{
		if (p_num_points < 2)
			p_num_points = 2;
		int old_size = points.size();
		points.resize(p_num_points);
		for (int idx = old_size; idx < points.size(); ++idx)
		{
			points[idx] = Vector2(0.0, 0.0);
		}
	}

	void SkwoxelFieldTwist::set_point(int idx, godot::Vector2 point)
	{
		if (idx >= 0 && idx < points.size())
			points[idx] = point;
	}

}
