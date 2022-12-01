#include "skwoxel_field_noise_dim.h"

#include "random.h"
#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldNoiseDim::_notification(int p_what) {
	}

	bool SkwoxelFieldNoiseDim::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(dimensions);
		SKWOXEL_SET_METHOD(seed);
		SKWOXEL_SET_METHOD(frequency);
		SKWOXEL_SET_METHOD(t);
		SKWOXEL_SET_METHOD(octaves);
		SKWOXEL_SET_METHOD(gain);
		SKWOXEL_SET_METHOD(lacunarity);

		return false;
	}

	bool SkwoxelFieldNoiseDim::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(dimensions);
		SKWOXEL_GET_METHOD(seed);
		SKWOXEL_GET_METHOD(frequency);
		SKWOXEL_GET_METHOD(t);
		SKWOXEL_GET_METHOD(octaves);
		SKWOXEL_GET_METHOD(gain);
		SKWOXEL_GET_METHOD(lacunarity);

		return false;
	}

	String SkwoxelFieldNoiseDim::_to_string() const {
		return "[Skwoxel field : NOISE DIM]";
	}

	void SkwoxelFieldNoiseDim::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldNoiseDim::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldNoiseDim::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldNoiseDim::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldNoiseDim, dimensions);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldNoiseDim, seed);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldNoiseDim, frequency);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldNoiseDim, t);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldNoiseDim, octaves);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldNoiseDim, gain);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldNoiseDim, lacunarity);

		ADD_PROPERTY(PropertyInfo(Variant::INT, "dimensions", PROPERTY_HINT_ENUM, "XYZ,XYZT"), "set_dimensions", "get_dimensions");
		SKWOXEL_ADD_PROPERTY(Variant::INT, seed);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, frequency);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, t);
		SKWOXEL_ADD_PROPERTY(Variant::INT, octaves);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, gain);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, lacunarity);
	}

	SkwoxelFieldNoiseDim::SkwoxelFieldNoiseDim() :
		SkwoxelField()
	{

	}

	SkwoxelFieldNoiseDim::~SkwoxelFieldNoiseDim()
	{

	}

	void SkwoxelFieldNoiseDim::pre_generate(bool randomize_seeds, int num_threads)
	{
		SkwoxelField::pre_generate(randomize_seeds, num_threads);

		int num_octaves = std::max(1, octaves);
		if (num_octaves != noises.size() || previous_seed != seed)
		{
			noises.resize(num_octaves);
			for (int n = 0; n < num_octaves; ++n)
			{
				noises[n] = std::make_shared<Noise>(randomize_seeds ? skwoxel::rand() : seed + n);
			}
		}

	}

	real_t SkwoxelFieldNoiseDim::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		real_t val = 0.0;
		real_t amplitude = 1.0;
		switch (dimensions)
		{
		case XYZT:
		{
			Vector4 xyzt(pos.x, pos.y, pos.z, t);
			xyzt *= frequency;
			for (auto noise : noises)
			{
				val += amplitude * noise->eval(xyzt.x, xyzt.y, xyzt.z, xyzt.w);
				xyzt *= lacunarity;
				amplitude *= gain;
			}
		}
			break;
		default:
		{
			Vector3 xyz(pos);
			xyz *= frequency;
			for (auto noise : noises)
			{
				val += amplitude * noise->eval(xyz.x, xyz.y, xyz.z);
				xyz *= lacunarity;
				amplitude *= gain;
			}
			break;
		}
		}
		return val;
	}

}
