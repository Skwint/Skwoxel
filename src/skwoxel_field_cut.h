#ifndef SKWOXEL_FIELD_CUT_H
#define SKWOXEL_FIELD_CUT_H

#include "skwoxel_field_add.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>

namespace skwoxel
{
	class SkwoxelFieldCut : public SkwoxelFieldAdd
	{
		GDCLASS(SkwoxelFieldCut, SkwoxelField);
	public:
		SkwoxelFieldCut();
		virtual ~SkwoxelFieldCut();

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
		void set_distance(real_t dist) { distance = dist; }

	private:
		godot::Vector3 normal;
		real_t distance;
	};
}

#endif
