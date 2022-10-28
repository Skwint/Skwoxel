#ifndef SKWOXEL_FIELD_PLANE_H
#define SKWOXEL_FIELD_PLANE_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>

namespace skwoxel
{
	class SkwoxelFieldPlane : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldPlane, SkwoxelField);
	public:
		SkwoxelFieldPlane();
		virtual ~SkwoxelFieldPlane();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		real_t strength(const godot::Vector3 & pos) const override;
		godot::Vector3 get_normal() const { return normal; };
		void set_normal(godot::Vector3 pos) { normal = pos; }
		real_t get_inner_distance() const { return inner_distance; };
		void set_inner_distance(real_t radius) { inner_distance = radius; }
		real_t get_outer_distance() const { return outer_distance; };
		void set_outer_distance(real_t radius) { outer_distance = radius; }
		real_t get_inner_strength() const { return inner_strength; };
		void set_inner_strength(real_t strength) { inner_strength = strength; }
		real_t get_outer_strength() const { return outer_strength; };
		void set_outer_strength(real_t strength) { outer_strength = strength; }

	private:
		godot::Vector3 normal;
		real_t inner_distance;
		real_t outer_distance;
		real_t inner_strength;
		real_t outer_strength;
	};
}

#endif
