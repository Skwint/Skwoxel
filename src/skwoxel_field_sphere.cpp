#include "skwoxel_field_sphere.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldSphere::_notification(int p_what) {
	}

	bool SkwoxelFieldSphere::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(inner_radius);
		SKWOXEL_SET_METHOD(outer_radius);
		SKWOXEL_SET_METHOD(inner_strength);
		SKWOXEL_SET_METHOD(outer_strength);

		return false;
	}

	bool SkwoxelFieldSphere::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(inner_radius);
		SKWOXEL_GET_METHOD(outer_radius);
		SKWOXEL_GET_METHOD(inner_strength);
		SKWOXEL_GET_METHOD(outer_strength);

		return false;
	}

	String SkwoxelFieldSphere::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelFieldSphere::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_radius"));
		list->push_back(PropertyInfo(Variant::FLOAT, "outer_radius"));
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_strength"));
		list->push_back(PropertyInfo(Variant::FLOAT, "outer_strength"));
	}

	bool SkwoxelFieldSphere::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldSphere::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldSphere::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldSphere, inner_radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldSphere, outer_radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldSphere, inner_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldSphere, outer_strength);

		//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "inner_radius", PROPERTY_HINT_RESOURCE_TYPE, VoxelStream::get_class_static()),
		//	"set_stream", "get_stream");
	}

	SkwoxelFieldSphere::SkwoxelFieldSphere() :
		SkwoxelField(),
		inner_radius(10.0),
		outer_radius(12.0),
		inner_strength(1.0),
		outer_strength(0.0)
	{

	}

	SkwoxelFieldSphere::~SkwoxelFieldSphere()
	{

	}

	real_t SkwoxelFieldSphere::strength(const Vector3& pos)
	{
		real_t rad = pos.length();
		if (rad < inner_radius)
			return inner_strength;
		else if (rad > outer_radius)
			return outer_strength;
		else
			return inner_strength + (outer_strength - inner_strength) * (rad - inner_radius) / (outer_radius - inner_radius);
	}

}
