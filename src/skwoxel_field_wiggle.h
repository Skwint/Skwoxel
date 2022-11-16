#ifndef SKWOXEL_FIELD_WIGGLE_H
#define SKWOXEL_FIELD_WIGGLE_H

#include "skwoxel_field_curve.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/curve3d.hpp>

namespace skwoxel
{
	class SkwoxelFieldWiggle : public SkwoxelFieldCurve
	{
		GDCLASS(SkwoxelFieldWiggle, SkwoxelFieldCurve);
	public:
		SkwoxelFieldWiggle();
		virtual ~SkwoxelFieldWiggle();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		void pre_generate(bool randomize_seeds);
		real_t strength(const godot::Vector3 & pos, const godot::Vector3& untransformed) const override;
		real_t get_end_radius() const { return end_radius; };
		void set_end_radius(real_t p_strength) { end_radius = p_strength; }

	private:
		real_t end_radius;
	};
}

#endif
