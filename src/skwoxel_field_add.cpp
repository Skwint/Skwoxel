#include "skwoxel_field_add.h"

using namespace godot;

namespace skwoxel
{

	SkwoxelFieldAdd::SkwoxelFieldAdd() :
		SkwoxelField()
	{

	}

	SkwoxelFieldAdd::~SkwoxelFieldAdd()
	{
	}

	void SkwoxelFieldAdd::_notification(int p_what) {
	}

	bool SkwoxelFieldAdd::_set(const StringName& p_name, const Variant& p_value) {
		return false;
	}

	bool SkwoxelFieldAdd::_get(const StringName& p_name, Variant& r_ret) const {
		return false;
	}

	String SkwoxelFieldAdd::_to_string() const {
		return "[Skwoxel field : CONSTANT]";
	}

	void SkwoxelFieldAdd::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldAdd::_property_can_revert(const StringName& p_name) const {
		return true;
	}

	bool SkwoxelFieldAdd::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldAdd::_bind_methods()
	{
	}

	real_t SkwoxelFieldAdd::strength(const Vector3 & pos, const Vector3& untransformed) const
	{
		real_t sum = 0.0;
		for (int ch = 0; ch < child_fields.size(); ch++)
		{
			sum += child_fields[ch]->strength(pos, untransformed);
		}
		return sum;
	}

}
