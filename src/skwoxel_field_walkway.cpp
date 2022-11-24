#include "skwoxel_field_walkway.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldWalkway::_notification(int p_what) {
	}

	bool SkwoxelFieldWalkway::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(up);
		SKWOXEL_SET_METHOD(top_strength);
		SKWOXEL_SET_METHOD(altitude);

		return false;
	}

	bool SkwoxelFieldWalkway::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(up);
		SKWOXEL_GET_METHOD(top_strength);
		SKWOXEL_GET_METHOD(altitude);

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
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldWalkway, up);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldWalkway, top_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldWalkway, altitude);

		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, up);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, top_strength);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, altitude);
	}

	SkwoxelFieldWalkway::SkwoxelFieldWalkway() :
		SkwoxelFieldCurve(),
		top_strength(-1.0),
		up(0.0, 1.0, 0.0),
		altitude(0.0)
	{

	}

	SkwoxelFieldWalkway::~SkwoxelFieldWalkway()
	{

	}

	real_t SkwoxelFieldWalkway::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		if (curve.is_valid() && bounds.has_point(pos))
		{
			Vector3 touch = curve->get_closest_point(pos);
			Vector3 delta = pos - touch;
			real_t rad = delta.length();
			real_t height = delta.dot(up) - altitude;
			real_t radial_multiplier = smooth_step(-blend, blend, radius - rad);
			real_t altitude_multiplier = smooth_step(-blend, blend, height);
			real_t str = Math::lerp(inner_strength, top_strength, altitude_multiplier);
			return str * radial_multiplier;
		}
		else
		{
			return 0.0;
		}
	}
}
