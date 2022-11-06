#include "skwoxel_field_torus.h"

#include <godot_cpp/classes/geometry3d.hpp>

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldTorus::_notification(int p_what) {
	}

	bool SkwoxelFieldTorus::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(centre);
		SKWOXEL_SET_METHOD(axis);
		SKWOXEL_SET_METHOD(major_radius);
		SKWOXEL_SET_METHOD(minor_radius);
		SKWOXEL_SET_METHOD(blend);
		SKWOXEL_SET_METHOD(inner_strength);

		return false;
	}

	bool SkwoxelFieldTorus::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(centre);
		SKWOXEL_GET_METHOD(axis);
		SKWOXEL_GET_METHOD(major_radius);
		SKWOXEL_GET_METHOD(minor_radius);
		SKWOXEL_GET_METHOD(blend);
		SKWOXEL_GET_METHOD(inner_strength);

		return false;
	}

	String SkwoxelFieldTorus::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelFieldTorus::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::VECTOR3, "centre"));
		list->push_back(PropertyInfo(Variant::VECTOR3, "axis"));
		list->push_back(PropertyInfo(Variant::FLOAT, "major_radius"));
		list->push_back(PropertyInfo(Variant::FLOAT, "minor_radius"));
		list->push_back(PropertyInfo(Variant::FLOAT, "blend"));
		list->push_back(PropertyInfo(Variant::FLOAT, "inner_strength"));
	}

	bool SkwoxelFieldTorus::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldTorus::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldTorus::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTorus, centre);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTorus, axis);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTorus, major_radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTorus, minor_radius);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTorus, blend);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTorus, inner_strength);
	}

	SkwoxelFieldTorus::SkwoxelFieldTorus() :
		SkwoxelField(),
		major_radius(6.0),
		minor_radius(3.0),
		blend(2.0),
		inner_strength(1.0)
	{
	}

	SkwoxelFieldTorus::~SkwoxelFieldTorus()
	{

	}

	void SkwoxelFieldTorus::pre_generate(bool randomize_seeds)
	{
		SkwoxelField::pre_generate(randomize_seeds);
		axis.normalize();
	}

	real_t SkwoxelFieldTorus::strength(const Vector3& pos) const
	{
		Vector3 relative = pos - centre;
		real_t height = relative.dot(axis);
		Vector3 norm = relative - height * axis;
		real_t radius = major_radius - norm.length();
		real_t rad = sqrt(radius * radius + height * height);
		real_t radial_multiplier = smooth_step(-blend, blend, rad - minor_radius);
		return Math::lerp(inner_strength, real_t(0.0), radial_multiplier);
	}

}
