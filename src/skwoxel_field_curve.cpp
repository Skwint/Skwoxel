#include "skwoxel_field_curve.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldCurve::_notification(int p_what) {
	}

	bool SkwoxelFieldCurve::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(curve);
		SKWOXEL_SET_METHOD(radius);
		SKWOXEL_SET_METHOD(blend);
		SKWOXEL_SET_METHOD(inner_strength);

		return false;
	}

	bool SkwoxelFieldCurve::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(curve);
		SKWOXEL_GET_METHOD(radius);
		SKWOXEL_GET_METHOD(blend);
		SKWOXEL_GET_METHOD(inner_strength);

		return false;
	}

	String SkwoxelFieldCurve::_to_string() const {
		return "[ wot? ]";
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
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, blend);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, inner_strength);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve3D"), "set_curve", "get_curve");
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, radius);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, blend);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, inner_strength);
	}

	SkwoxelFieldCurve::SkwoxelFieldCurve() :
		SkwoxelField(),
		radius(10.0),
		blend(2.0),
		inner_strength(1.0)
	{

	}

	SkwoxelFieldCurve::~SkwoxelFieldCurve()
	{

	}

	void SkwoxelFieldCurve::pre_generate(bool randomize_seeds)
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

	real_t SkwoxelFieldCurve::strength(const Vector3& pos) const
	{
		if (curve.is_valid() && aabb.has_point(pos))
		{
			Vector3 touch = curve->get_closest_point(pos);
			float rad = (pos - touch).length();
			float radial_multiplier = smooth_step(-blend, blend, rad - radius);
			return Math::lerp((float)inner_strength, 0.0f, radial_multiplier);
		}
		else
		{
			return 0.0;
		}
	}
}
