#include "skwoxel_field_turbulence.h"

#include "random.h"
#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldTurbulence::_notification(int p_what) {
	}

	bool SkwoxelFieldTurbulence::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(direction);
		SKWOXEL_SET_METHOD(noise);

		return false;
	}

	bool SkwoxelFieldTurbulence::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(direction);
		SKWOXEL_GET_METHOD(noise);

		return false;
	}

	String SkwoxelFieldTurbulence::_to_string() const {
		return "[Skwoxel field : TURBULENCE]";
	}

	void SkwoxelFieldTurbulence::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldTurbulence::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldTurbulence::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldTurbulence::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTurbulence, direction);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTurbulence, noise);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "noise", PROPERTY_HINT_RESOURCE_TYPE, FastNoiseLite::get_class_static(),
			PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT),
			"set_noise", "get_noise");
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, direction);
	}

	SkwoxelFieldTurbulence::SkwoxelFieldTurbulence() :
		SkwoxelFieldAdd()
	{

	}

	SkwoxelFieldTurbulence::~SkwoxelFieldTurbulence()
	{

	}

	void SkwoxelFieldTurbulence::pre_generate(bool randomize_seeds)
	{
		SkwoxelField::pre_generate(randomize_seeds);
		if (randomize_seeds)
		{
			noise->set_seed(skwoxel::rand());
		}
	}

	real_t SkwoxelFieldTurbulence::strength(const Vector3& pos) const
	{
		real_t str = noise->get_noise_3dv(pos);
		return SkwoxelFieldAdd::strength(pos + str * direction);
	}
}
