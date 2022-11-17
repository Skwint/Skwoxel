#ifndef SKWOXEL_ADD_H
#define SKWOXEL_ADD_H

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include "skwoxel_field.h"

namespace skwoxel
{
	class SkwoxelFieldAdd : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldAdd, SkwoxelField);
	public:
		SkwoxelFieldAdd();
		virtual ~SkwoxelFieldAdd();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		void trigger(const godot::Vector3& pos, const godot::Vector3& untransformed) override;
		real_t strength(const godot::Vector3 & pos) const override;
	};
}

#endif
