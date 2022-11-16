#include "skwoxel_field_mirror.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldMirror::_notification(int p_what) {
	}

	bool SkwoxelFieldMirror::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(normal);
		SKWOXEL_SET_METHOD(distance);
		SKWOXEL_SET_METHOD(blend);
		SKWOXEL_SET_METHOD(amplify);

		return false;
	}

	bool SkwoxelFieldMirror::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(normal);
		SKWOXEL_GET_METHOD(distance);
		SKWOXEL_GET_METHOD(blend);
		SKWOXEL_GET_METHOD(amplify);

		return false;
	}

	String SkwoxelFieldMirror::_to_string() const {
		return "[Skwoxel field : MIRROR]";
	}

	void SkwoxelFieldMirror::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldMirror::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldMirror::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldMirror::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldMirror, normal);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldMirror, distance);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldMirror, blend);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldMirror, amplify);

		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, normal);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, distance);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, blend);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, amplify);
	}

	SkwoxelFieldMirror::SkwoxelFieldMirror() :
		SkwoxelFieldAdd(),
		distance(0.0),
		blend(2.0),
		amplify(1.0)
	{

	}

	SkwoxelFieldMirror::~SkwoxelFieldMirror()
	{

	}

	real_t SkwoxelFieldMirror::strength(const Vector3 & pos, const Vector3& untransformed) const
	{
		real_t rad = pos.dot(normal) - distance;
		if (rad > blend)
		{
			return SkwoxelFieldAdd::strength(pos, untransformed);
		}

		Vector3 reflected = pos - 2.0 * rad * normal;
		real_t reflection = amplify * SkwoxelFieldAdd::strength(reflected, untransformed);
		if (rad < -blend)
		{
			return reflection;
		}
		else
		{
			real_t multiplier = smooth_step(-blend, blend, rad);
			return Math::lerp(reflection, SkwoxelFieldAdd::strength(pos, untransformed), multiplier);
		}
	}

}
