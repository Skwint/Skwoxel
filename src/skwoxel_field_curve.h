#ifndef SKWOXEL_FIELD_CURVE_H
#define SKWOXEL_FIELD_CURVE_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/curve3d.hpp>

namespace skwoxel
{
	class SkwoxelFieldCurve : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldCurve, SkwoxelField);
	public:
		SkwoxelFieldCurve();
		virtual ~SkwoxelFieldCurve();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		real_t strength(const godot::Vector3 & pos) const override;
		godot::Ref<godot::Curve3D> get_curve() const { return curve; };
		void set_curve(const godot::Ref<godot::Curve3D> cur) { curve = cur; }
		real_t get_radius() const { return radius; };
		void set_radius(real_t p_radius) { radius = p_radius; }
		real_t get_blend() const { return blend; };
		void set_blend(real_t p_blend) { blend = MAX(0.1, p_blend); }
		real_t get_inner_strength() const { return inner_strength; };
		void set_inner_strength(real_t p_strength) { inner_strength = p_strength; }

	protected:
		godot::Ref<godot::Curve3D> curve;
		real_t radius;
		real_t blend;
		real_t inner_strength;
	};
}

#endif
