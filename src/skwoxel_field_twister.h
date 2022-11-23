#ifndef SKWOXEL_FIELD_TWISTER_H
#define SKWOXEL_FIELD_TWISTER_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/core/binder_common.hpp>

#include "skwoxel_helpers.h"

namespace skwoxel
{
	class SkwoxelFieldTwister : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldTwister, SkwoxelField);
	public:
		SkwoxelFieldTwister();
		virtual ~SkwoxelFieldTwister();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		void pre_generate(bool randomize_seeds) override;
		real_t strength(const godot::Vector3 & pos) const override;

		int get_num_points() const { return points.size(); }
		void set_num_points(int p_num_points);
		godot::Vector2 get_point(int idx) const { if (idx >= 0 && idx < points.size()) return points[idx]; return godot::Vector2(0.0, 0.0); }
		void set_point(int idx, godot::Vector2 point);
		real_t get_length() const { return length; }
		void set_length(real_t p_length) { length = p_length; }
		real_t get_step() const { return step; };
		void set_step(real_t p_step) { step = MAX(1.0, p_step); }
		real_t get_radius() const { return radius; };
		void set_radius(real_t p_radius) { radius = p_radius; }
		real_t get_blend() const { return blend; };
		void set_blend(real_t p_blend) { blend = MAX(0.1, p_blend); }
		real_t get_inner_strength() const { return inner_strength; };
		void set_inner_strength(real_t p_strength) { inner_strength = p_strength; }

	protected:
		std::vector<godot::Vector2> points;
		real_t length;
		real_t step;
		real_t radius;
		real_t blend;
		real_t inner_strength;
	private:
		real_t cache_step;
		std::vector<godot::Vector3> cache;
	};
}

#endif
