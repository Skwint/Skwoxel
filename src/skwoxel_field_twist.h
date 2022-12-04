#ifndef SKWOXEL_FIELD_TWIST_H
#define SKWOXEL_FIELD_TWIST_H

#include "skwoxel_field_add.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/core/binder_common.hpp>

#include "skwoxel_helpers.h"

namespace skwoxel
{
	class SkwoxelFieldTwist : public SkwoxelFieldAdd
	{
		GDCLASS(SkwoxelFieldTwist, SkwoxelFieldAdd);
	public:
		SkwoxelFieldTwist();
		virtual ~SkwoxelFieldTwist();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		real_t strength(const godot::Vector3 & pos, const godot::Vector3 & untransformed, int thread_num) const override;

		int get_num_points() const { return points.size(); }
		void set_num_points(int p_num_points);
		godot::Vector2 get_point(int idx) const { if (idx >= 0 && idx < points.size()) return points[idx]; return godot::Vector2(0.0, 0.0); }
		void set_point(int idx, godot::Vector2 point);
		real_t get_length() const { return length; }
		void set_length(real_t p_length) { length = p_length; }

	protected:
		std::vector<godot::Vector2> points;
		real_t length;
	};
}

#endif
