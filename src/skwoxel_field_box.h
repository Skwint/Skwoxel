#ifndef SKWOXEL_FIELD_BOX_H
#define SKWOXEL_FIELD_BOX_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/aabb.hpp>
#include <godot_cpp/core/binder_common.hpp>

namespace skwoxel
{
	class SkwoxelFieldBox : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldBox, SkwoxelField);
	public:
		SkwoxelFieldBox();
		virtual ~SkwoxelFieldBox();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		real_t strength(const godot::Vector3 & pos, const godot::Vector3 & untransformed, int thread_num) const override;
		godot::AABB get_aabb() const { return aabb; };
		void set_aabb(godot::AABB p_aabb) { aabb = p_aabb; }
		real_t get_blend() const { return blend; };
		void set_blend(real_t p_blend) { blend = std::max((real_t)0.1, p_blend); }
		real_t get_inner_strength() const { return inner_strength; };
		void set_inner_strength(real_t p_strength) { inner_strength = p_strength; }

	private:
		godot::AABB aabb;
		real_t blend;
		real_t inner_strength;
	};
}

#endif
