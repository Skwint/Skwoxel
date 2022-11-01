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
		SKWOXEL_SET_METHOD(inner_radius);
		SKWOXEL_SET_METHOD(outer_radius);
		SKWOXEL_SET_METHOD(inner_strength);
		SKWOXEL_SET_METHOD(outer_strength);

		return false;
	}

	bool SkwoxelFieldCurve::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(curve);
		SKWOXEL_GET_METHOD(inner_radius);
		SKWOXEL_GET_METHOD(outer_radius);
		SKWOXEL_GET_METHOD(inner_strength);
		SKWOXEL_GET_METHOD(outer_strength);

		return false;
	}

	String SkwoxelFieldCurve::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelFieldCurve::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_radius"));
		list->push_back(PropertyInfo(Variant::FLOAT, "outer_radius"));
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_strength"));
		list->push_back(PropertyInfo(Variant::FLOAT, "outer_strength"));
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
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, inner_radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, outer_radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, inner_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCurve, outer_strength);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve3D"), "set_curve", "get_curve");
	}

	SkwoxelFieldCurve::SkwoxelFieldCurve() :
		SkwoxelField(),
		inner_radius(10.0),
		outer_radius(12.0),
		inner_strength(1.0),
		outer_strength(0.0)
	{

	}

	SkwoxelFieldCurve::~SkwoxelFieldCurve()
	{

	}

	real_t SkwoxelFieldCurve::strength(const Vector3& pos) const
	{
		if (curve.is_valid())
		{
			Vector3 touch = curve->get_closest_point(pos);
			auto rad = (pos - touch).length();
			if (rad < inner_radius)
			{
				return inner_strength;
			}
			else if (rad > outer_radius)
			{
				return outer_strength;
			}
			else
			{
				real_t r = (rad - inner_radius) / (outer_radius - inner_radius);
				return (1.0 - r) * inner_strength + r * outer_strength;
			}
		}
		else
		{
			return 0.0;
		}
	}
}
