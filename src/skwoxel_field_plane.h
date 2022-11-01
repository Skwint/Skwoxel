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
		real_t get_distance() const { return distance; };
		void set_distance(real_t p_distance) { distance = p_distance; }
		real_t get_blend() const { return blend; };
		void set_blend(real_t p_blend) { blend = p_blend; }
		real_t get_inner_strength() const { return inner_strength; };
		void set_inner_strength(real_t p_strength) { inner_strength = p_strength; }

	private:
		godot::Vector3 normal;
		real_t distance;
		real_t blend;
		real_t inner_strength;
		real_t outer_strength;
	};
}

#endif
