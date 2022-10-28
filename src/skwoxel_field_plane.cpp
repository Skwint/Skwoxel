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
		SKWOXEL_SET_METHOD(inner_distance);
		SKWOXEL_SET_METHOD(outer_distance);
		SKWOXEL_SET_METHOD(inner_strength);
		SKWOXEL_SET_METHOD(outer_strength);

		return false;
	}

	bool SkwoxelFieldPlane::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(normal);
		SKWOXEL_GET_METHOD(inner_distance);
		SKWOXEL_GET_METHOD(outer_distance);
		SKWOXEL_GET_METHOD(inner_strength);
		SKWOXEL_GET_METHOD(outer_strength);

		return false;
	}

	String SkwoxelFieldPlane::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelFieldPlane::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::VECTOR3, "normal"));
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_distance"));
		list->push_back(PropertyInfo(Variant::FLOAT, "outer_distance"));
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_strength"));
		list->push_back(PropertyInfo(Variant::FLOAT, "outer_strength"));
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
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldPlane, inner_distance);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldPlane, outer_distance);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldPlane, inner_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldPlane, outer_strength);
	}

	SkwoxelFieldPlane::SkwoxelFieldPlane() :
		SkwoxelField(),
		inner_distance(0.0),
		outer_distance(2.0),
		inner_strength(1.0),
		outer_strength(0.0)
	{

	}

	SkwoxelFieldPlane::~SkwoxelFieldPlane()
	{

	}

	real_t SkwoxelFieldPlane::strength(const Vector3& pos) const
	{
		real_t rad = pos.dot(normal);
		if (rad < inner_distance)
		{
			return inner_strength;
		}
		else if (rad > outer_distance)
		{
			return outer_strength;
		}
		else
		{
			real_t r = (rad - inner_distance) / (outer_distance - inner_distance);
			return (1.0 - r) * inner_strength + r * outer_strength;
		}
	}

}
