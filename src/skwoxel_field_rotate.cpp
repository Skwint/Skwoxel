#include "skwoxel_field_rotate.h"

#include "random.h"
#include "skwoxel_helpers.h"

using namespace godot;
using std::vector;

namespace skwoxel
{
	void SkwoxelFieldRotate::_notification(int p_what) {
	}

	bool SkwoxelFieldRotate::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(axis);
		SKWOXEL_SET_METHOD(angle);
		SKWOXEL_SET_METHOD(twist);

		return false;
	}

	bool SkwoxelFieldRotate::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(axis);
		SKWOXEL_GET_METHOD(angle);
		SKWOXEL_GET_METHOD(twist);

		return false;
	}

	String SkwoxelFieldRotate::_to_string() const {
		return "[Skwoxel field : ROTATE]";
	}

	void SkwoxelFieldRotate::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldRotate::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldRotate::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldRotate::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldRotate, axis);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldRotate, angle);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldRotate, twist);

		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, axis);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, angle);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, twist);
	}

	SkwoxelFieldRotate::SkwoxelFieldRotate() :
		SkwoxelFieldAdd(),
		axis(0.0, 1.0, 0.0),
		angle(0.0),
		twist(0.0)
	{

	}

	SkwoxelFieldRotate::~SkwoxelFieldRotate()
	{

	}
	
	void SkwoxelFieldRotate::pre_generate(bool randomize_seeds)
	{
		SkwoxelFieldAdd::pre_generate(randomize_seeds);
		axis_normalized = axis.normalized();
		rotator.set_axis_angle(axis_normalized, -angle);
	}

	void SkwoxelFieldRotate::trigger(const Vector3& pos, const Vector3& untransformed)
	{
		return SkwoxelFieldAdd::trigger(rotator.xform(pos), untransformed);
	}

	real_t SkwoxelFieldRotate::strength(const Vector3& pos) const
	{
		if (twist != 0.0) // intentional equate of real
		{
			godot::Basis twisted_rotator;
			twisted_rotator.set_axis_angle(axis_normalized, -angle * twist * pos.dot(axis_normalized));
			return SkwoxelFieldAdd::strength(twisted_rotator.xform(pos));
		}
		return SkwoxelFieldAdd::strength(rotator.xform(pos));
	}
}
