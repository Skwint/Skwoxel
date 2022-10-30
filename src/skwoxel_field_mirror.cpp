#include "skwoxel_field_mirror.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldMirror::_notification(int p_what) {
	}

	bool SkwoxelFieldMirror::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(normal);
		SKWOXEL_SET_METHOD(distance);
		SKWOXEL_SET_METHOD(blend);

		return false;
	}

	bool SkwoxelFieldMirror::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(normal);
		SKWOXEL_GET_METHOD(distance);
		SKWOXEL_GET_METHOD(blend);

		return false;
	}

	String SkwoxelFieldMirror::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelFieldMirror::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::VECTOR3, "normal"));
		list->push_back(PropertyInfo(Variant::FLOAT, "distance"));
		list->push_back(PropertyInfo(Variant::FLOAT, "blend"));
	}

	bool SkwoxelFieldMirror::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldMirror::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldMirror::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldMirror, normal);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldMirror, distance);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldMirror, blend);
	}

	SkwoxelFieldMirror::SkwoxelFieldMirror() :
		SkwoxelFieldAdd(),
		distance(0.0),
		blend(1.0)
	{

	}

	SkwoxelFieldMirror::~SkwoxelFieldMirror()
	{

	}

	real_t SkwoxelFieldMirror::strength(const Vector3& pos) const
	{
		real_t rad = pos.dot(normal) - distance;
		if (rad > blend)
		{
			return SkwoxelFieldAdd::strength(pos);
		}

		Vector3 reflected = pos - 2.0 * rad * normal;
		if (rad < -blend)
		{
			return SkwoxelFieldAdd::strength(reflected);
		}
		else
		{
			real_t r = (rad + blend) / (2.0 * blend);
			return (1.0 - r) * SkwoxelFieldAdd::strength(reflected) + r * SkwoxelFieldAdd::strength(pos);
		}
	}

}