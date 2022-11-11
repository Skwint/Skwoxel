#include "skwoxel_field_walkway.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldWalkway::_notification(int p_what) {
	}

	bool SkwoxelFieldWalkway::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(curve);
		SKWOXEL_SET_METHOD(radius);
		SKWOXEL_SET_METHOD(up);
		SKWOXEL_SET_METHOD(floor_strength);
		SKWOXEL_SET_METHOD(air_strength);
		SKWOXEL_SET_METHOD(blend);

		return false;
	}

	bool SkwoxelFieldWalkway::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(curve);
		SKWOXEL_GET_METHOD(radius);
		SKWOXEL_GET_METHOD(up);
		SKWOXEL_GET_METHOD(floor_strength);
		SKWOXEL_GET_METHOD(air_strength);
		SKWOXEL_GET_METHOD(blend);

		return false;
	}

	String SkwoxelFieldWalkway::_to_string() const {
		return "[Skwoxel field : WALKWAY]";
	}

	void SkwoxelFieldWalkway::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldWalkway::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldWalkway::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldWalkway::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldWalkway, curve);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldWalkway, radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldWalkway, up);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldWalkway, floor_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldWalkway, air_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldWalkway, blend);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve3D"), "set_curve", "get_curve");
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, radius);
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, up);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, floor_strength);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, air_strength);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, blend);
	}

	SkwoxelFieldWalkway::SkwoxelFieldWalkway() :
		SkwoxelField(),
		radius(10.0),
		floor_strength(1.0),
		air_strength(-1.0),
		blend(2.0),
		up(0.0, 1.0, 0.0)
	{

	}

	SkwoxelFieldWalkway::~SkwoxelFieldWalkway()
	{

	}

	void SkwoxelFieldWalkway::pre_generate(bool randomize_seeds)
	{
		SkwoxelField::pre_generate(randomize_seeds);

		real_t border = radius + blend + normal_offset;
		Vector3 border_size(border, border, border);
		AABB little_box;
		little_box.set_size(2.0 * border_size);
		if (curve.is_valid())
		{
			aabb.set_position(curve->get_point_position(0));
			aabb.set_size(Vector3(0.0, 0.0, 0.0));
			int num = curve->get_point_count();
			for (int pp = 0; pp < num; ++pp)
			{
				Vector3 pos = curve->get_point_position(pp);
				little_box.set_position(pos - border_size);
				aabb.merge_with(little_box);
				if (pp != 0)
				{
					Vector3 in = pos + curve->get_point_in(pp);
					little_box.set_position(in - border_size);
					aabb.merge_with(little_box);
				}
				if (pp < num - 1)
				{
					Vector3 out = pos + curve->get_point_out(pp);
					little_box.set_position(out - border_size);
					aabb.merge_with(little_box);
				}
			}
		}
	}

	real_t SkwoxelFieldWalkway::strength(const Vector3& pos) const
	{
		if (curve.is_valid() && aabb.has_point(pos))
		{
			Vector3 touch = curve->get_closest_point(pos);
			Vector3 delta = pos - touch;
			float rad = delta.length();
			float altitude = delta.dot(up);
			float radial_multiplier = smooth_step(-blend, blend, radius - rad);
			float altitude_multiplier = smooth_step(-blend, blend, altitude);
			return radial_multiplier * Math::lerp(floor_strength, air_strength, altitude_multiplier);
		}
		else
		{
			return 0.0;
		}
	}
}
