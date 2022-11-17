#include "skwoxel_field_sphere.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldSphere::_notification(int p_what) {
	}

	bool SkwoxelFieldSphere::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(centre);
		SKWOXEL_SET_METHOD(radius);
		SKWOXEL_SET_METHOD(blend);
		SKWOXEL_SET_METHOD(inner_strength);

		return false;
	}

	bool SkwoxelFieldSphere::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(centre);
		SKWOXEL_GET_METHOD(radius);
		SKWOXEL_GET_METHOD(blend);
		SKWOXEL_GET_METHOD(inner_strength);

		return false;
	}

	String SkwoxelFieldSphere::_to_string() const {
		return "[Skwoxel field : SPHERE]";
	}

	void SkwoxelFieldSphere::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldSphere::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldSphere::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldSphere::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldSphere, centre);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldSphere, radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldSphere, blend);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldSphere, inner_strength);

		// Properties.
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, centre);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, radius);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, blend);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, inner_strength);
	}

	SkwoxelFieldSphere::SkwoxelFieldSphere() :
		SkwoxelField(),
		radius(10.0),
		blend(2.0),
		inner_strength(1.0)
	{

	}

	SkwoxelFieldSphere::~SkwoxelFieldSphere()
	{

	}

	real_t SkwoxelFieldSphere::strength(const Vector3& pos) const
	{
		real_t rad = (pos - centre).length();
		real_t radial_multiplier = smooth_step(-blend, blend, radius - rad);
		return inner_strength * radial_multiplier;
	}

}
