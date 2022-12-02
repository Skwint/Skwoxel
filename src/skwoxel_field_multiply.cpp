#include "skwoxel_field_multiply.h"

#include "skwoxel_helpers.h"

using namespace godot;
using std::vector;

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

	real_t SkwoxelFieldMultiply::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		real_t prod = 1.0;
		for (int ch = 0; ch < child_fields.size(); ch++)
		{
			if (child_fields[ch]->is_enabled())
				prod *= child_fields[ch]->strength(pos, untransformed, thread_num);
		}
		return prod;
	}
}
