#include "skwoxel_field_capsule.h"

#include <godot_cpp/classes/geometry3d.hpp>

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldCapsule::_notification(int p_what) {
	}

	bool SkwoxelFieldCapsule::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(point1);
		SKWOXEL_SET_METHOD(point2);
		SKWOXEL_SET_METHOD(inner_radius);
		SKWOXEL_SET_METHOD(outer_radius);
		SKWOXEL_SET_METHOD(inner_strength);
		SKWOXEL_SET_METHOD(outer_strength);

		return false;
	}

	bool SkwoxelFieldCapsule::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(point1);
		SKWOXEL_GET_METHOD(point2);
		SKWOXEL_GET_METHOD(inner_radius);
		SKWOXEL_GET_METHOD(outer_radius);
		SKWOXEL_GET_METHOD(inner_strength);
		SKWOXEL_GET_METHOD(outer_strength);

		return false;
	}

	String SkwoxelFieldCapsule::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelFieldCapsule::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::VECTOR3, "point1"));
		list->push_back(PropertyInfo(Variant::VECTOR3, "point2"));
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_radius"));
		list->push_back(PropertyInfo(Variant::FLOAT, "outer_radius"));
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_strength"));
		list->push_back(PropertyInfo(Variant::FLOAT, "outer_strength"));
	}

	bool SkwoxelFieldCapsule::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldCapsule::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldCapsule::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsule, point1);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsule, point2);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsule, inner_radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsule, outer_radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsule, inner_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsule, outer_strength);

		//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "inner_radius", PROPERTY_HINT_RESOURCE_TYPE, VoxelStream::get_class_static()),
		//	"set_stream", "get_stream");
	}

	SkwoxelFieldCapsule::SkwoxelFieldCapsule() :
		SkwoxelField(),
		inner_radius(5.0),
		outer_radius(10.0),
		inner_strength(1.0),
		outer_strength(0.0)
	{
	}

	SkwoxelFieldCapsule::~SkwoxelFieldCapsule()
	{

	}

	real_t SkwoxelFieldCapsule::strength(const Vector3& pos) const
	{
		Vector3 close = Geometry3D::get_singleton()->get_closest_point_to_segment(pos, point1, point2);
		real_t rad = (pos - close).length();
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

}
