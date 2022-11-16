#ifndef SKWOXEL_FIELD_CONSTANT_H
#define SKWOXEL_FIELD_CONSTANT_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>

namespace skwoxel
{
	class SkwoxelFieldConstant : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldConstant, SkwoxelField);
	public:
		SkwoxelFieldConstant();
		virtual ~SkwoxelFieldConstant();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		real_t strength(const godot::Vector3 & pos, const godot::Vector3& untransformed) const override;
		real_t get_constant() const { return constant; };
		void set_constant(real_t val) { constant = val; }

	private:
		real_t constant;
	};
}

#endif
