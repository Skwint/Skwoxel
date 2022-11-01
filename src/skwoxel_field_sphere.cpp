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
		return "[ wot? ]";
	}

	void SkwoxelFieldSphere::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::VECTOR3, "centre"));
		list->push_back(PropertyInfo(Variant::FLOAT, "radius"));
		list->push_back(PropertyInfo(Variant::FLOAT, "blend"));
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_strength"));
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
		real_t radial_multiplier = Math::smoothstep(-blend, blend, rad - radius);
		return Math::lerp(inner_strength, real_t(0.0), radial_multiplier);
	}

}
