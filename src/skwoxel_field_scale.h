#ifndef SKWOXEL_FIELD_SCALE_H
#define SKWOXEL_FIELD_SCALE_H

#include "skwoxel_field_add.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace skwoxel
{
	class SkwoxelFieldScale : public SkwoxelFieldAdd
	{
		GDCLASS(SkwoxelFieldScale, SkwoxelFieldAdd);
	public:
		SkwoxelFieldScale();
		virtual ~SkwoxelFieldScale();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		real_t strength(const godot::Vector3 & pos) const override;
		godot::Vector3 get_scale() const { return scale; };
		void set_scale(godot::Vector3 p_scale) { scale = p_scale; }

	private:
		godot::Vector3 scale;
	};
}

#endif
