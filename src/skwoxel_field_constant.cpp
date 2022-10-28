#include "skwoxel_field_constant.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldConstant::_notification(int p_what) {
	}

	bool SkwoxelFieldConstant::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(constant);

		return false;
	}

	bool SkwoxelFieldConstant::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(constant);

		return false;
	}

	String SkwoxelFieldConstant::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelFieldConstant::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::FLOAT, "constant"));
	}

	bool SkwoxelFieldConstant::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldConstant::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldConstant::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldConstant, constant);
	}

	SkwoxelFieldConstant::SkwoxelFieldConstant() :
		SkwoxelField(),
		constant(0.0)
	{

	}

	SkwoxelFieldConstant::~SkwoxelFieldConstant()
	{

	}

	real_t SkwoxelFieldConstant::strength(const Vector3& pos) const
	{
		real_t sum = SkwoxelField::strength(pos);
		return sum + constant;
	}

}
