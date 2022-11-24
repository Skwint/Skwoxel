#include "skwoxel_field_capsule_sliced.h"

#include <godot_cpp/classes/geometry3d.hpp>

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldCapsuleSliced::_notification(int p_what) {
	}

	bool SkwoxelFieldCapsuleSliced::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(up);
		SKWOXEL_SET_METHOD(top_strength);
		SKWOXEL_SET_METHOD(altitude);

		return false;
	}

	bool SkwoxelFieldCapsuleSliced::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(up);
		SKWOXEL_GET_METHOD(top_strength);
		SKWOXEL_GET_METHOD(altitude);

		return false;
	}

	String SkwoxelFieldCapsuleSliced::_to_string() const {
		return "[Skwoxel field : CAPSULE SLICED]";
	}

	void SkwoxelFieldCapsuleSliced::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldCapsuleSliced::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldCapsuleSliced::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldCapsuleSliced::_bind_methods() {
		// Methods
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsuleSliced, up);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsuleSliced, top_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsuleSliced, altitude);

		// Properties
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, up);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, top_strength);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, altitude);
	}

	SkwoxelFieldCapsuleSliced::SkwoxelFieldCapsuleSliced() :
		SkwoxelFieldCapsule(),
		top_strength(-1.0),
		up(0.0, 1.0, 0.0),
		altitude(0.0)
	{
	}

	SkwoxelFieldCapsuleSliced::~SkwoxelFieldCapsuleSliced()
	{

	}

	real_t SkwoxelFieldCapsuleSliced::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		Vector3 close = Geometry3D::get_singleton()->get_closest_point_to_segment(pos, point1, point2);
		Vector3 delta = pos - close;
		real_t rad = delta.length();
		real_t height = delta.dot(up) - altitude;
		real_t radial_multiplier = smooth_step(-blend, blend, radius - rad);
		real_t altitude_multiplier = smooth_step(-blend, blend, height);
		real_t str = Math::lerp(inner_strength, top_strength, altitude_multiplier);
		return str * radial_multiplier;
	}
}
