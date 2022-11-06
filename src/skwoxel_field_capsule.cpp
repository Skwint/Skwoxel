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
		SKWOXEL_SET_METHOD(radius);
		SKWOXEL_SET_METHOD(blend);
		SKWOXEL_SET_METHOD(inner_strength);

		return false;
	}

	bool SkwoxelFieldCapsule::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(point1);
		SKWOXEL_GET_METHOD(point2);
		SKWOXEL_GET_METHOD(radius);
		SKWOXEL_GET_METHOD(blend);
		SKWOXEL_GET_METHOD(inner_strength);

		return false;
	}

	String SkwoxelFieldCapsule::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelFieldCapsule::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::VECTOR3, "point1"));
		list->push_back(PropertyInfo(Variant::VECTOR3, "point2"));
		list->push_back(PropertyInfo(Variant::FLOAT, "radius"));
		list->push_back(PropertyInfo(Variant::FLOAT, "blend"));
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_strength"));
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
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsule, radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsule, blend);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCapsule, inner_strength);

		//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "radius", PROPERTY_HINT_RESOURCE_TYPE, VoxelStream::get_class_static()),
		//	"set_stream", "get_stream");
	}

	SkwoxelFieldCapsule::SkwoxelFieldCapsule() :
		SkwoxelField(),
		radius(5.0),
		blend(2.0),
		inner_strength(1.0)
	{
	}

	SkwoxelFieldCapsule::~SkwoxelFieldCapsule()
	{

	}

	real_t SkwoxelFieldCapsule::strength(const Vector3& pos) const
	{
		Vector3 close = Geometry3D::get_singleton()->get_closest_point_to_segment(pos, point1, point2);
		real_t rad = (pos - close).length();
		real_t radial_multiplier = smooth_step(-blend, blend, rad - radius);
		return Math::lerp(inner_strength, real_t(0.0), radial_multiplier);
	}

}
