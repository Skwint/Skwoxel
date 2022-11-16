#include "skwoxel_field_select.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{

	SkwoxelFieldSelect::SkwoxelFieldSelect() :
		SkwoxelField(),
		criteria(STRONGEST)
	{

	}

	SkwoxelFieldSelect::~SkwoxelFieldSelect()
	{
	}

	void SkwoxelFieldSelect::_notification(int p_what) {
	}

	bool SkwoxelFieldSelect::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(criteria);
		return false;
	}

	bool SkwoxelFieldSelect::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(criteria);
		return false;
	}

	String SkwoxelFieldSelect::_to_string() const {
		return "[Skwoxel field : SELECT]";
	}

	void SkwoxelFieldSelect::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldSelect::_property_can_revert(const StringName& p_name) const {
		return true;
	}

	bool SkwoxelFieldSelect::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldSelect::_bind_methods()
	{
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldSelect, criteria);

		ADD_PROPERTY(PropertyInfo(Variant::INT, "criteria", PROPERTY_HINT_ENUM, "Strongest,Highest,Lowest"), "set_criteria", "get_criteria");
	}

	real_t SkwoxelFieldSelect::strength(const Vector3 & pos, const Vector3& untransformed) const
	{
		if (child_fields.size() == 0)
			return 0.0;

		real_t lowest = 9999.0;
		real_t highest = -lowest;
		for (int ch = 0; ch < child_fields.size(); ch++)
		{
			real_t val = child_fields[ch]->strength(pos, untransformed);
			highest = MAX(highest, val);
			lowest = MIN(lowest, val);
		}
		switch (criteria)
		{
		case STRONGEST:
			if (abs(highest) > abs(lowest))
				return highest;
			else
				return lowest;
		case HIGHEST:
			return highest;
		case LOWEST:
			return lowest;
		default:
			return 0.0;
		}
	}

}
