#include "skwoxel_field_noise.h"

#include "random.h"
#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldNoise::_notification(int p_what) {
	}

	bool SkwoxelFieldNoise::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(noise);

		return false;
	}

	bool SkwoxelFieldNoise::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(noise);

		return false;
	}

	String SkwoxelFieldNoise::_to_string() const {
		return "[ wot? ]";
	}

	void SkwoxelFieldNoise::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldNoise::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldNoise::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldNoise::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldNoise, noise);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "noise", PROPERTY_HINT_RESOURCE_TYPE, FastNoiseLite::get_class_static(),
			PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_EDITOR_INSTANTIATE_OBJECT),
			"set_noise", "get_noise");
	}

	SkwoxelFieldNoise::SkwoxelFieldNoise() :
		SkwoxelField()
	{

	}

	SkwoxelFieldNoise::~SkwoxelFieldNoise()
	{

	}

	void SkwoxelFieldNoise::pre_generate(bool randomize_seeds)
	{
		if (randomize_seeds)
		{
			noise->set_seed(skwoxel::rand());
		}
	}

	real_t SkwoxelFieldNoise::strength(const Vector3& pos) const
	{
		return noise->get_noise_3dv(pos);
	}

}
