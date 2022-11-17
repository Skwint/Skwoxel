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
		return "[Skwoxel field : MULTIPLY]";
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

	void SkwoxelFieldMultiply::trigger(const Vector3& pos, const Vector3& untransformed)
	{
		for (int ch = 0; ch < child_fields.size(); ch++)
		{
			child_fields[ch]->trigger(pos, untransformed);
		}
	}

	real_t SkwoxelFieldMultiply::strength(const Vector3& pos) const
	{
		// maths says the product of no numbers at all is 1.
		// I say a multiply field with no children is intuitively 0.
		if (child_fields.size() == 0)
			return 0.0;

		real_t prod = 1.0;
		for (int ch = 0; ch < child_fields.size(); ch++)
		{
			prod *= child_fields[ch]->strength(pos);
		}
		return prod;
	}
}
