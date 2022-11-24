#include "skwoxel_field_box.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldBox::_notification(int p_what) {
	}

	bool SkwoxelFieldBox::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(aabb);
		SKWOXEL_SET_METHOD(blend);
		SKWOXEL_SET_METHOD(inner_strength);

		return false;
	}

	bool SkwoxelFieldBox::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(aabb);
		SKWOXEL_GET_METHOD(blend);
		SKWOXEL_GET_METHOD(inner_strength);

		return false;
	}

	String SkwoxelFieldBox::_to_string() const {
		return "[Skwoxel field : BOX]";
	}

	void SkwoxelFieldBox::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldBox::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldBox::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldBox::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldBox, aabb);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldBox, blend);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldBox, inner_strength);

		SKWOXEL_ADD_PROPERTY(Variant::AABB, aabb);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, blend);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, inner_strength);
	}

	SkwoxelFieldBox::SkwoxelFieldBox() :
		SkwoxelField(),
		blend(2.0),
		inner_strength(1.0)
	{

	}

	SkwoxelFieldBox::~SkwoxelFieldBox()
	{

	}

	real_t SkwoxelFieldBox::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		Vector3 low = aabb.get_position();
		Vector3 high = low + aabb.get_size();
		real_t x_up = smooth_step(-blend, blend, pos.x - low.x);
		real_t x_down = smooth_step(-blend, blend, high.x - pos.x);
		real_t y_up = smooth_step(-blend, blend, pos.y - low.y);
		real_t y_down = smooth_step(-blend, blend, high.y - pos.y);
		real_t z_up = smooth_step(-blend, blend, pos.z - low.z);
		real_t z_down = smooth_step(-blend, blend, high.z - pos.z);
		return inner_strength * x_up * x_down * y_up * y_down * z_up * z_down;
	}

}
