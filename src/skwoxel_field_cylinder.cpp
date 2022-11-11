#include "skwoxel_field_cylinder.h"

#include <godot_cpp/classes/geometry3d.hpp>

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldCylinder::_notification(int p_what) {
	}

	bool SkwoxelFieldCylinder::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(point1);
		SKWOXEL_SET_METHOD(point2);
		SKWOXEL_SET_METHOD(radius);
		SKWOXEL_SET_METHOD(blend);
		SKWOXEL_SET_METHOD(inner_strength);

		return false;
	}

	bool SkwoxelFieldCylinder::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(point1);
		SKWOXEL_GET_METHOD(point2);
		SKWOXEL_GET_METHOD(radius);
		SKWOXEL_GET_METHOD(blend);
		SKWOXEL_GET_METHOD(inner_strength);

		return false;
	}

	String SkwoxelFieldCylinder::_to_string() const {
		return "[Skwoxel field : CAPSULE]";
	}

	void SkwoxelFieldCylinder::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldCylinder::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldCylinder::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldCylinder::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCylinder, point1);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCylinder, point2);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCylinder, radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCylinder, blend);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCylinder, inner_strength);

		// Properties
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, point1);
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, point2);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, radius);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, blend);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, inner_strength);
	}

	SkwoxelFieldCylinder::SkwoxelFieldCylinder() :
		SkwoxelField(),
		radius(5.0),
		blend(2.0),
		inner_strength(1.0)
	{
	}

	SkwoxelFieldCylinder::~SkwoxelFieldCylinder()
	{

	}

	void SkwoxelFieldCylinder::pre_generate(bool randomize_seeds)
	{
		SkwoxelField::pre_generate(randomize_seeds);

		length = (point2 - point1).length();
		if (length > 1e-20)
			normal = (point2 - point1) / length;
	}

	real_t SkwoxelFieldCylinder::strength(const Vector3& pos) const
	{
		auto rel = pos - point1;
		if (length <= 1e-20f) {
			return 0.0;
		}

		real_t dist = rel.dot(normal);

		// infinite cylinder:
		Vector3 close = point1 + dist * normal;
		real_t rad = (pos - close).length() - radius;
		real_t radial_multiplier = smooth_step(blend, -blend, rad);

		// capping planes:
		real_t end1 = - dist;
		real_t end2 = dist - length;
		real_t cap_multiplier = smooth_step(blend, -blend, MAX(end1, end2));

		// combined:
		return Math::lerp(real_t(0.0), inner_strength, cap_multiplier * radial_multiplier);
	}

}
