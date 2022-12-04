#include "skwoxel_field_twister.h"

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
	void SkwoxelFieldTwister::_notification(int p_what) {
	}

	bool SkwoxelFieldTwister::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(curve);
		SKWOXEL_SET_METHOD(length);
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
		SKWOXEL_GET_METHOD(curve);
		SKWOXEL_GET_METHOD(length);
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
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, curve);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, length);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, blend);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, inner_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, slice);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, slice_altitude);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, top_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTwister, up);
		ClassDB::bind_method(D_METHOD("auto_control_points"), &SkwoxelFieldTwister::auto_control_points);

		// Properties
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve2D"), "set_curve", "get_curve");
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, length);
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
		radius(5.0),
		length(100.0),
		blend(2.0),
		inner_strength(1.0)
	{
	}

	SkwoxelFieldTwister::~SkwoxelFieldTwister()
	{

	}

	void SkwoxelFieldTwister::auto_control_points()
	{
		if (curve.is_valid() && curve->get_point_count() > 1)
		{
			int num = curve->get_point_count();
			Vector2 delta = curve->get_point_position(1) - curve->get_point_position(0);
			curve->set_point_out(0, 0.4 * delta);
			delta = curve->get_point_position(num - 1) - curve->get_point_position(num - 2);
			curve->set_point_out(num - 1, -0.4 * delta);
			for (int pp = 1; pp < num - 1; ++pp)
			{
				Vector2 before = curve->get_point_position(pp - 1);
				Vector2 after = curve->get_point_position(pp + 1);
				delta = after - before;
				curve->set_point_in(pp, -0.2 * delta);
				curve->set_point_out(pp, 0.2 * delta);
			}
		}
	}

	void SkwoxelFieldTwister::pre_generate(bool randomize_seeds, int num_threads)
	{
		SkwoxelField::pre_generate(randomize_seeds, num_threads);

		cache.resize(0);
		if (curve.is_valid())
		{
			int num = curve->get_point_count();
			real_t section_length = length / num;
			int section_count = int(ceil(section_length));
			real_t section_step = section_length / section_count;
			cache.resize(section_count * num + 1);
			for (int pp = 0; pp < num - 1; ++pp)
			{
				Vector2 pos = curve->get_point_position(pp);
				Vector2 out = pos + curve->get_point_out(pp);
				Vector2 pos2 = curve->get_point_position(pp + 1);
				Vector2 in = pos2 + curve->get_point_in(pp + 1);
				for (int step = 0; step < section_count; ++step)
				{
					int idx = pp * section_count + step;
					real_t t = real_t(step) / real_t(section_count);
					Vector2 interp = pos.bezier_interpolate(out, in, pos2, t);
					Vector3 pos;
					pos.x = interp.x * cos(interp.y);
					pos.y = interp.x * sin(interp.y);
					pos.z = (real_t)idx;
					cache[idx] = pos;
				}
			}
			Vector2 cpos = curve->get_point_position(num - 1);
			cache[section_count * num] = Vector3(cpos.x, cpos.y, section_count * num);
		}
		up.normalize();
	}

	real_t SkwoxelFieldTwister::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		real_t range = radius + blend;
		real_t first_index_f = max(real_t(0.0), pos.z - range);
		real_t last_index_f = min(length, pos.z + range);
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
			close = get_closest_point_to_segment(pos, cache[index], cache[index + 1]);
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
}
