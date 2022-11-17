#include "skwoxel_field_scale.h"

#include "random.h"
#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldScale::_notification(int p_what) {
	}

	bool SkwoxelFieldScale::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(scale);

		return false;
	}

	bool SkwoxelFieldScale::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(scale);

		return false;
	}

	String SkwoxelFieldScale::_to_string() const {
		return "[Skwoxel field : SCALE]";
	}

	void SkwoxelFieldScale::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldScale::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldScale::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldScale::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldScale, scale);
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, scale);
	}

	SkwoxelFieldScale::SkwoxelFieldScale() :
		SkwoxelFieldAdd(),
		scale(1.0, 1.0, 1.0)
	{

	}

	SkwoxelFieldScale::~SkwoxelFieldScale()
	{

	}

	void SkwoxelFieldScale::trigger(const Vector3& pos, const Vector3& untransformed)
	{
		return SkwoxelFieldAdd::trigger(pos / scale, untransformed);
	}

	real_t SkwoxelFieldScale::strength(const Vector3& pos) const
	{
		return SkwoxelFieldAdd::strength(pos / scale);
	}
}
