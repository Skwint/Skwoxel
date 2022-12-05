#include "skwoxel_field_curve.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldCurve::_notification(int p_what) {
	}

	bool SkwoxelFieldCurve::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(curve);
		SKWOXEL_SET_METHOD(start_radius);
		SKWOXEL_SET_METHOD(end_radius);
		SKWOXEL_SET_METHOD(blend);
		SKWOXEL_SET_METHOD(inner_strength);
		SKWOXEL_SET_METHOD(sliced);
		SKWOXEL_SET_METHOD(outer_strength);
		SKWOXEL_SET_METHOD(altitude);
		SKWOXEL_SET_METHOD(up);

		return false;
	}

	bool SkwoxelFieldCurve::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(curve);
		SKWOXEL_GET_METHOD(start_radius);
		SKWOXEL_GET_METHOD(end_radius);
		SKWOXEL_GET_METHOD(blend);
		SKWOXEL_GET_METHOD(inner_strength);
		SKWOXEL_GET_METHOD(sliced);
		SKWOXEL_GET_METHOD(outer_strength);
		SKWOXEL_GET_METHOD(altitude);
		SKWOXEL_GET_METHOD(up);

		return false;
	}

	String SkwoxelFieldCurve::_to_string() const {
		return "[Skwoxel field : CURVE]";
	}

	bool SkwoxelFieldCurve::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldCurve::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldCurve::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, curve);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, start_radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, end_radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, blend);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, inner_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, sliced);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, outer_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, altitude);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, up);
		ClassDB::bind_method(D_METHOD("balance_control_points"), &SkwoxelFieldCurve::balance_control_points);
		ClassDB::bind_method(D_METHOD("set_all_control_points", "direction"), &SkwoxelFieldCurve::set_all_control_points, Variant::VECTOR3);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve3D"), "set_curve", "get_curve");
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, start_radius);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, end_radius);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, blend);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, inner_strength);
		SKWOXEL_ADD_PROPERTY(Variant::BOOL, sliced);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, outer_strength);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, altitude);
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, up);
	}

	SkwoxelFieldCurve::SkwoxelFieldCurve() :
		SkwoxelField()
	{

	}

	SkwoxelFieldCurve::~SkwoxelFieldCurve()
	{

	}

	void SkwoxelFieldCurve::calculate_bounds()
	{
		real_t border = std::max(start_radius, end_radius) + blend + normal_offset;
		Vector3 border_size(border, border, border);
		AABB little_box;
		little_box.set_size(2.0 * border_size);
		if (curve.is_valid())
		{
			bounds.set_position(curve->get_point_position(0));
			bounds.set_size(Vector3(0.0, 0.0, 0.0));
			int num = curve->get_point_count();
			for (int pp = 0; pp < num; ++pp)
			{
				Vector3 pos = curve->get_point_position(pp);
				little_box.set_position(pos - border_size);
				bounds.merge_with(little_box);
				if (pp != 0)
				{
					Vector3 in = pos + curve->get_point_in(pp);
					little_box.set_position(in - border_size);
					bounds.merge_with(little_box);
				}
				if (pp < num - 1)
				{
					Vector3 out = pos + curve->get_point_out(pp);
					little_box.set_position(out - border_size);
					bounds.merge_with(little_box);
				}
			}
		}
	}

	// We often want as smooth a curve as possible, which is generally when the control
	// points are evenly distributes, so lets automate that for those that want it
	// This is very approximate - it's only correct for straight lines
	void SkwoxelFieldCurve::balance_control_points()
	{
		if (curve.is_valid())
		{
			int num = curve->get_point_count();
			for (int pp = 0; pp < num - 1; ++pp)
			{
				Vector3 p0 = curve->get_point_position(pp);
				Vector3 p1 = curve->get_point_position(pp + 1);
				Vector3 dir = p1 - p0;
				real_t dist = dir.length() / 3.0;
				Vector3 ctrlout = curve->get_point_out(pp);
				Vector3 ctrlin = curve->get_point_in(pp + 1);
				ctrlout.normalize();
				ctrlin.normalize();
				ctrlout *= dist;
				ctrlin *= dist;
				curve->set_point_out(pp, ctrlout);
				curve->set_point_in(pp + 1, ctrlin);
			}
		}
	}

	void SkwoxelFieldCurve::set_all_control_points(const Vector3& direction)
	{
		if (curve.is_valid())
		{
			int num = curve->get_point_count();
			for (int pp = 0; pp < num; ++pp)
			{
				curve->set_point_out(pp, direction);
				curve->set_point_in(pp, -direction);
			}
		}
	}

	void SkwoxelFieldCurve::pre_generate(bool randomize_seeds, int num_threads)
	{
		SkwoxelField::pre_generate(randomize_seeds, num_threads);
		quick.set_curve(curve);
		calculate_bounds();
	}

	real_t SkwoxelFieldCurve::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		if (bounds.has_point(pos))
		{
			auto touch = quick.seek_closest(pos);
			if (touch.distance_squared >= 0.0)
			{
				real_t offset = touch.offset;
				Vector3 delta = pos - touch.pos;
				real_t rad = delta.length();
				real_t interpolated_radius = Math::lerp(start_radius, end_radius, offset);
				real_t radial_multiplier = smooth_step(-blend, blend, interpolated_radius - rad);
				real_t str = inner_strength;
				if (sliced)
				{
					real_t height = delta.dot(up) - altitude;
					real_t altitude_multiplier = smooth_step(-blend, blend, height);
					str = Math::lerp(inner_strength, outer_strength, altitude_multiplier);
				}
				return str * radial_multiplier;
			}
		}
		return 0.0;
	}
}
