#ifndef SKWOXEL_FIELD_ROTATE_H
#define SKWOXEL_FIELD_ROTATE_H

#include "skwoxel_field_add.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/basis.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace skwoxel
{
	class SkwoxelFieldRotate : public SkwoxelFieldAdd
	{
		GDCLASS(SkwoxelFieldRotate, SkwoxelFieldAdd);
	public:
		SkwoxelFieldRotate();
		virtual ~SkwoxelFieldRotate();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		void pre_generate(bool randomize_seeds);
		real_t strength(const godot::Vector3 & pos) const override;
		godot::Vector3 get_axis() const { return axis; };
		void set_axis(godot::Vector3 p_axis) { axis = p_axis; }
		real_t get_angle() const { return angle; };
		void set_angle(real_t p_angle) { angle = p_angle; }

	private:
		mutable godot::Basis rotator;
		godot::Vector3 axis;
		real_t angle;
	};
}

#endif
