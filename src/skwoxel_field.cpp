#include "skwoxel_field.h"

using namespace godot;

namespace skwoxel
{

	SkwoxelField::SkwoxelField() : Node3D()
	{

	}

	SkwoxelField::~SkwoxelField()
	{

	}

	void SkwoxelField::_notification(int p_what) {
	}

	bool SkwoxelField::_set(const StringName& p_name, const Variant& p_value) {
		return false;
	}

	bool SkwoxelField::_get(const StringName& p_name, Variant& r_ret) const {
		return false;
	}

	String SkwoxelField::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelField::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelField::_property_can_revert(const StringName& p_name) const {
		return true;
	}

	bool SkwoxelField::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelField::_bind_methods()
	{
	}

	real_t SkwoxelField::strength(const Vector3& pos)
	{
		return 0.0;
	}

}
