#ifndef SKWOXEL_FIELD_WALKWAY_H
#define SKWOXEL_FIELD_WALKWAY_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/curve3d.hpp>

namespace skwoxel
{
	class SkwoxelFieldWalkway : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldWalkway, SkwoxelField);
	public:
		SkwoxelFieldWalkway();
		virtual ~SkwoxelFieldWalkway();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
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
		godot::Vector3 get_up() const { return up; };
		void set_up(godot::Vector3 p_up) { up = p_up; }
		real_t get_floor_strength() const { return floor_strength; };
		void set_floor_strength(real_t p_floor_strength) { floor_strength = p_floor_strength; }
		real_t get_air_strength() const { return air_strength; };
		void set_air_strength(real_t p_air_strength) { air_strength = p_air_strength; }
		real_t get_blend() const { return blend; };
		void set_blend(real_t p_blend) { blend = MAX(0.1, p_blend); }

	private:
		godot::Ref<godot::Curve3D> curve;
		real_t radius;
		real_t blend;
		real_t floor_strength;
		real_t air_strength;
		godot::Vector3 up;
	};
}

#endif
