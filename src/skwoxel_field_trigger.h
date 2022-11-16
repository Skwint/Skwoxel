#ifndef SKWOXEL_FIELD_TRIGGER_H
#define SKWOXEL_FIELD_TRIGGER_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/wrapped.hpp>

#include "lotsa.h"

using godot::StringName; // This is bad, but GDCLASS is being unfriendly

namespace skwoxel
{
	class SkwoxelFieldTrigger : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldTrigger, SkwoxelField);
	public:
		SkwoxelFieldTrigger();
		virtual ~SkwoxelFieldTrigger();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		void pre_generate(bool randomize_seeds) override;
		real_t strength(const godot::Vector3 & pos, const godot::Vector3& untransformed) const override;
		void post_generate() override;
		godot::Vector3 get_point() const { return point; }
		void set_point(const godot::Vector3& p_point) { point = p_point; }

	protected:
		godot::Vector3 point;

	private:
		mutable godot::Vector3 closest;
		mutable real_t distance_squared;
	};
}

#endif
