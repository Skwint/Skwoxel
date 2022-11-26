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

		void pre_generate(bool randomize_seeds, int num_threads) override;
		real_t strength(const godot::Vector3 & pos, const godot::Vector3 & untransformed, int thread_num) const override;

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
		real_t get_sliced_up() const { return sliced_up; }
		void set_sliced_up(bool p_sliced_up) { sliced_up = p_sliced_up; sliced_in = false; sliced_out = false; }
		real_t get_sliced_in() const { return sliced_in; }
		void set_sliced_in(bool p_sliced_in) { sliced_in = p_sliced_in; sliced_out = false; sliced_up = false; }
		real_t get_sliced_out() const { return sliced_out; }
		void set_sliced_out(bool p_sliced_out) { sliced_out = p_sliced_out; sliced_in = false; sliced_up = false; }
		SKWOXEL_INLINE_SET_GET(real_t, slice_altitude);
		SKWOXEL_INLINE_SET_GET(real_t, top_strength);
		SKWOXEL_INLINE_SET_GET(godot::Vector3, up);

	protected:
		std::vector<godot::Vector2> points;
		real_t length;
		real_t step;
		real_t radius;
		real_t blend;
		real_t inner_strength;
		bool sliced_up = false;
		bool sliced_in = false;
		bool sliced_out = false;
		real_t slice_altitude = 0.0;
		real_t top_strength = -1.0;
		godot::Vector3 up = godot::Vector3(0.0, 1.0, 0.0);
	private:
		real_t cache_step;
		std::vector<godot::Vector3> cache;
	};
}

#endif
