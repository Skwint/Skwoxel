#ifndef SKWOXEL_FIELD_NOISE_DIM_H
#define SKWOXEL_FIELD_NOISE_DIM_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/noise.hpp>
#include <godot_cpp/classes/ref.hpp>

#include "skwoxel_helpers.h"
#include "noise.h"

namespace skwoxel
{
	class SkwoxelFieldNoiseDim : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldNoiseDim, SkwoxelField);
	private:
		enum Dimensions
		{
			XYZ,
			XYZT
		};
	public:
		SkwoxelFieldNoiseDim();
		virtual ~SkwoxelFieldNoiseDim();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		void pre_generate(bool randomize_seeds, int num_threads);
		real_t strength(const godot::Vector3 & pos, const godot::Vector3 & untransformed, int thread_num) const override;

		SKWOXEL_INLINE_SET_GET(int, seed);
		SKWOXEL_INLINE_SET_GET(real_t, frequency);
		SKWOXEL_INLINE_SET_GET(int, octaves);
		SKWOXEL_INLINE_SET_GET(real_t, gain);
		SKWOXEL_INLINE_SET_GET(real_t, lacunarity);
		int get_dimensions() const { return (int)dimensions; };
		void set_dimensions(int p_dimensions) { dimensions = (Dimensions)p_dimensions; }
		SKWOXEL_INLINE_SET_GET(real_t, t);

	private:
		std::vector<std::shared_ptr<Noise>> noises;
		Dimensions dimensions = XYZ;
		int seed = 0;
		int previous_seed = 0;
		real_t frequency = 0.01;
		real_t t = 0.0;
		int octaves = 3;
		real_t gain = 0.5;
		real_t lacunarity = 2.0;
	};
}

#endif
