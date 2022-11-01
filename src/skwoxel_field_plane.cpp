#include "skwoxel_field_plane.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldPlane::_notification(int p_what) {
	}

	bool SkwoxelFieldPlane::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(normal);
		SKWOXEL_SET_METHOD(distance);
		SKWOXEL_SET_METHOD(blend);
		SKWOXEL_SET_METHOD(inner_strength);

		return false;
	}

	bool SkwoxelFieldPlane::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(normal);
		SKWOXEL_GET_METHOD(distance);
		SKWOXEL_GET_METHOD(blend);
		SKWOXEL_GET_METHOD(inner_strength);

		return false;
	}

	String SkwoxelFieldPlane::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelFieldPlane::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::VECTOR3, "normal"));
		list->push_back(PropertyInfo(Variant::FLOAT, "distance"));
		list->push_back(PropertyInfo(Variant::FLOAT, "blend"));
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_strength"));
	}

	bool SkwoxelFieldPlane::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldPlane::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldPlane::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldPlane, normal);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldPlane, distance);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldPlane, blend);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldPlane, inner_strength);
	}

	SkwoxelFieldPlane::SkwoxelFieldPlane() :
		SkwoxelField(),
		distance(0.0),
		blend(2.0),
		inner_strength(1.0)
	{

	}

	SkwoxelFieldPlane::~SkwoxelFieldPlane()
	{

	}

	real_t SkwoxelFieldPlane::strength(const Vector3& pos) const
	{
		real_t rad = pos.dot(normal);
		real_t radial_multiplier = Math::smoothstep(-blend, blend, rad - distance);
		return Math::lerp(inner_strength, real_t(0.0), radial_multiplier);
	}

}
