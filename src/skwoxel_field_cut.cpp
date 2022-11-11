#include "skwoxel_field_cut.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldCut::_notification(int p_what) {
	}

	bool SkwoxelFieldCut::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(normal);
		SKWOXEL_SET_METHOD(distance);

		return false;
	}

	bool SkwoxelFieldCut::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(normal);
		SKWOXEL_GET_METHOD(distance);

		return false;
	}

	String SkwoxelFieldCut::_to_string() const {
		return "[Skwoxel field : CUT]";
	}

	void SkwoxelFieldCut::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldCut::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldCut::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldCut::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCut, normal);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldCut, distance);

		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, normal);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, distance);
	}

	SkwoxelFieldCut::SkwoxelFieldCut() :
		SkwoxelFieldAdd(),
		distance(0.0)
	{

	}

	SkwoxelFieldCut::~SkwoxelFieldCut()
	{

	}

	real_t SkwoxelFieldCut::strength(const Vector3& pos) const
	{
		real_t rad = pos.dot(normal);
		if (rad < distance)
			return 0.0;
		else
			return SkwoxelFieldAdd::strength(pos);
	}

}
