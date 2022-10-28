#include "skwoxel_field_multiply.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldMultiply::_notification(int p_what) {
	}

	bool SkwoxelFieldMultiply::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;

		return false;
	}

	bool SkwoxelFieldMultiply::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;

		return false;
	}

	String SkwoxelFieldMultiply::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelFieldMultiply::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldMultiply::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldMultiply::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldMultiply::_bind_methods() {
		// Methods.
	}

	SkwoxelFieldMultiply::SkwoxelFieldMultiply() :
		SkwoxelField()
	{

	}

	SkwoxelFieldMultiply::~SkwoxelFieldMultiply()
	{

	}

	real_t SkwoxelFieldMultiply::strength(const Vector3& pos) const
	{
		if (num_child_fields == 0)
			return 0.0;

		real_t prod = 1.0;
		for (int ch = 0; ch < num_child_fields; ch++)
		{
			prod *= child_fields[ch]->strength(pos);
		}
		return prod;
	}
}
