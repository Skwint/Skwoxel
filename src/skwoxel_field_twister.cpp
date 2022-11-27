#include "skwoxel_field_twister.h"

#include <algorithm>
#include <godot_cpp/classes/geometry3d.hpp>
#include <godot_cpp/variant/Vector2.hpp>

#include "skwoxel_helpers.h"

using namespace godot;
using namespace std;

namespace skwoxel
{
	void SkwoxelFieldTwister::_notification(int p_what) {
	}

	bool SkwoxelFieldTwister::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(num_points);
		SKWOXEL_SET_METHOD(length);
		SKWOXEL_SET_METHOD(step);
		SKWOXEL_SET_METHOD(radius);
		SKWOXEL_SET_METHOD(blend);
		SKWOXEL_SET_METHOD(inner_strength);
		SKWOXEL_SET_METHOD(slice);
		SKWOXEL_SET_METHOD(slice_altitude);
		SKWOXEL_SET_METHOD(top_strength);
		SKWOXEL_SET_METHOD(up);

		return false;
	}

	bool SkwoxelFieldTwister::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(num_points);
		SKWOXEL_GET_METHOD(length);
		SKWOXEL_GET_METHOD(step);
		SKWOXEL_GET_METHOD(radius);
		SKWOXEL_GET_METHOD(blend);
		SKWOXEL_GET_METHOD(inner_strength);
		SKWOXEL_GET_METHOD(slice);
		SKWOXEL_GET_METHOD(slice_altitude);
		SKWOXEL_GET_METHOD(top_strength);
		SKWOXEL_GET_METHOD(up);

		return false;
	}

	String SkwoxelFieldTwister::_to_string() const {
		return "[Skwoxel field : TWISTER]";
	}

	void SkwoxelFieldTwister::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldTwister::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldTwister::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldTwister::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, num_points);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, length);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, step);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, blend);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, inner_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, slice);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, slice_altitude);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, top_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, up);
		ClassDB::bind_method(D_METHOD("get_point", "index"), &SkwoxelFieldTwister::get_point, Variant::INT);
		ClassDB::bind_method(D_METHOD("set_point", "index", "pos"), &SkwoxelFieldTwister::set_point, Variant::INT, Variant::VECTOR2);

		// Properties
		SKWOXEL_ADD_PROPERTY(Variant::INT, num_points);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, length);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, step);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, radius);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, blend);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, inner_strength);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "slice", PROPERTY_HINT_ENUM, "none,up,in,out"), "set_slice", "get_slice");
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, slice_altitude);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, top_strength);
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, up);
	}

	SkwoxelFieldTwister::SkwoxelFieldTwister() :
		SkwoxelField(),
		length(10.0),
		step(1.0),
		radius(5.0),
		blend(2.0),
		inner_strength(1.0)
	{
		set_num_points(2);
	}

	SkwoxelFieldTwister::~SkwoxelFieldTwister()
	{

	}

	void SkwoxelFieldTwister::pre_generate(bool randomize_seeds, int num_threads)
	{
		SkwoxelField::pre_generate(randomize_seeds, num_threads);

		real_t hop_length = length / (points.size() - 1);
		int steps_per_hop = (int)ceil(hop_length / step);
		cache_step = hop_length / (real_t)steps_per_hop;
		real_t ctrl_step = hop_length * 0.4;

		cache.resize(steps_per_hop * (points.size() - 1) + 1);
		for (int idx = 0; idx < points.size() - 1; ++idx)
		{
			Vector3 from(points[idx].x, points[idx].y, hop_length * idx);
			Vector3 out = from;
			out.z += ctrl_step;
			Vector3 to(points[idx + 1].x, points[idx + 1].y, hop_length * (idx + 1));
			Vector3 in = to;
			in.z -= ctrl_step;
			for (int s = 0; s < steps_per_hop; ++s)
			{
				cache[idx * steps_per_hop + s] = from.bezier_interpolate(out, in, to, (real_t)s / (real_t)steps_per_hop);
			}
		}
		cache[cache.size() - 1] = Vector3(points[points.size() - 1].x, points[points.size() - 1].y, length);

		up.normalize();
	}

	real_t SkwoxelFieldTwister::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		real_t first_index_f = max(real_t(0.0), (pos.z - radius) / cache_step);
		real_t last_index_f = min(length, (pos.z + radius)) / cache_step;
		int first_index = int(first_index_f);
		int last_index = int(ceil(last_index_f));
		if (last_index < 0)
			return 0.0;
		if (first_index > cache.size() - 1)
			return 0.0;
		Vector3 close;
		Vector3 closest;
		real_t dist_sq = 99999999.0;
		for (int index = first_index; index < last_index - 1; ++index)
		{
			close = Geometry3D::get_singleton()->get_closest_point_to_segment(pos, cache[index], cache[index + 1]);
			real_t new_dist_sq = (pos - close).length_squared();
			if (new_dist_sq < dist_sq)
			{
				dist_sq = new_dist_sq;
				closest = close;
			}
		}
		real_t rad = sqrt(dist_sq);
		real_t radial_multiplier = smooth_step(-blend, blend, radius - rad);
		real_t str = inner_strength;
		if (slice != NONE)
		{
			Vector3 delta = pos - closest;
			real_t height;
			if (slice == UP)
			{
				height = delta.dot(up) - slice_altitude;
			}
			else
			{
				Vector2 clopos(closest.x, closest.y);
				Vector2 relpos(delta.x, delta.y);
				clopos.normalize();
				height = relpos.dot(clopos) - slice_altitude;
				if (slice == IN)
				{
					height *= -1.0;
				}
			}
			real_t altitude_multiplier = smooth_step(-blend, blend, height);
			str = Math::lerp(inner_strength, top_strength, altitude_multiplier);
		}
		return str * radial_multiplier;
	}

	void SkwoxelFieldTwister::set_num_points(int p_num_points)
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

	void SkwoxelFieldTwister::set_point(int idx, godot::Vector2 point)
	{
		if (idx >= 0 && idx < points.size())
			points[idx] = point;
	}

}
