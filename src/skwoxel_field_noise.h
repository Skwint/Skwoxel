#ifndef SKWOXEL_FIELD_NOISE_H
#define SKWOXEL_FIELD_NOISE_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/fast_noise_lite.hpp>
#include <godot_cpp/classes/noise.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace skwoxel
{
	class SkwoxelFieldNoise : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldNoise, SkwoxelField);
	public:
		SkwoxelFieldNoise();
		virtual ~SkwoxelFieldNoise();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		void pre_generate(bool randomize_seeds);
		real_t strength(const godot::Vector3 & pos, const godot::Vector3& untransformed) const override;
		godot::Ref<godot::FastNoiseLite> get_noise() const { return noise; };
		void set_noise(godot::Ref<godot::FastNoiseLite> val) { noise = val; }

	private:
		godot::Ref<godot::FastNoiseLite> noise;
	};
}

#endif
