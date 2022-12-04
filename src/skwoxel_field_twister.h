#ifndef SKWOXEL_FIELD_TWISTER_H
#define SKWOXEL_FIELD_TWISTER_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/curve2d.hpp>

#include "skwoxel_helpers.h"

namespace skwoxel
{
	class SkwoxelFieldTwister : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldTwister, SkwoxelField);
	public:
		struct Point
		{
			godot::Vector3 pos;
			real_t offset;
			real_t distance_squared;
		};
	private:
		enum Slice
		{
			NONE,
			UP,
			IN,
			OUT
		};
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

		godot::Ref<godot::Curve2D> get_curve() const { return curve; };
		void set_curve(const godot::Ref<godot::Curve2D> cur) { curve = cur; }
		real_t get_blend() const { return blend; };
		void set_blend(real_t p_blend) { blend = MAX(0.1, p_blend); }
		int get_slice() const { return (int)slice; };
		void set_slice(int p_slice) { slice = (Slice)p_slice; }
		SKWOXEL_INLINE_SET_GET(real_t, radius);
		SKWOXEL_INLINE_SET_GET(real_t, inner_strength);
		SKWOXEL_INLINE_SET_GET(real_t, length);
		SKWOXEL_INLINE_SET_GET(real_t, slice_altitude);
		SKWOXEL_INLINE_SET_GET(real_t, top_strength);
		SKWOXEL_INLINE_SET_GET(godot::Vector3, up);
		void auto_control_points();


	protected:
		godot::Ref<godot::Curve2D> curve;
		real_t radius;
		real_t length;
		real_t blend;
		real_t inner_strength;
		Slice slice = NONE;
		real_t slice_altitude = 0.0;
		real_t top_strength = -1.0;
		godot::Vector3 up = godot::Vector3(0.0, 1.0, 0.0);
	private:
		std::vector<godot::Vector3> cache;
	};
}

#endif
