#ifndef SKWOXEL_FIELD_SPHERE_H
#define SKWOXEL_FIELD_SPHERE_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>

namespace skwoxel
{
	class SkwoxelFieldSphere : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldSphere, SkwoxelField);
	public:
		SkwoxelFieldSphere();
		virtual ~SkwoxelFieldSphere();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		real_t strength(const godot::Vector3 & pos) override;
		real_t get_inner_radius() const { return inner_radius; };
		void set_inner_radius(real_t radius) { inner_radius = radius; }
		real_t get_outer_radius() const { return outer_radius; };
		void set_outer_radius(real_t radius) { outer_radius = radius; }
		real_t get_inner_strength() const { return inner_strength; };
		void set_inner_strength(real_t strength) { inner_strength = strength; }
		real_t get_outer_strength() const { return outer_strength; };
		void set_outer_strength(real_t strength) { outer_strength = strength; }

	private:
		real_t inner_radius;
		real_t outer_radius;
		real_t inner_strength;
		real_t outer_strength;
	};
}

#endif
