#ifndef SKWOXEL_SELECT_H
#define SKWOXEL_SELECT_H

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/node3d.hpp>

#include "skwoxel_field.h"

namespace skwoxel
{
	class SkwoxelFieldSelect : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldSelect, SkwoxelField);
	public:
		SkwoxelFieldSelect();
		virtual ~SkwoxelFieldSelect();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();


		virtual real_t strength(const godot::Vector3 & pos, const godot::Vector3 & untransformed, int thread_num) const override;;
		int get_criteria() const { return criteria; };
		void set_criteria(int p_criteria) { criteria = p_criteria; }

	protected:
		enum Criteria
		{
			STRONGEST,
			HIGHEST,
			LOWEST
		};
		int criteria;
	};
}

#endif
