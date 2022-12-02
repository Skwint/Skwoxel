#ifndef SKWOXEL_FIELD_CURVE_H
#define SKWOXEL_FIELD_CURVE_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/aabb.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/curve3d.hpp>

#include "quick_curve.h"
#include "skwoxel_helpers.h"

namespace skwoxel
{
	class SkwoxelFieldCurve : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldCurve, SkwoxelField);
	public:
		SkwoxelFieldCurve();
		virtual ~SkwoxelFieldCurve();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		void pre_generate(bool randomize_seeds, int num_threads) override;
		real_t strength(const godot::Vector3 & pos, const godot::Vector3 & untransformed, int thread_num) const override;
		godot::Ref<godot::Curve3D> get_curve() const { return curve; };
		void set_curve(const godot::Ref<godot::Curve3D> cur) { curve = cur; }
		SKWOXEL_INLINE_SET_GET(real_t, start_radius);
		SKWOXEL_INLINE_SET_GET(real_t, end_radius);
		real_t get_blend() const { return blend; };
		void set_blend(real_t p_blend) { blend = MAX(0.1, p_blend); }
		SKWOXEL_INLINE_SET_GET(real_t, inner_strength);
		SKWOXEL_INLINE_SET_GET(bool, sliced);
		SKWOXEL_INLINE_SET_GET(real_t, outer_strength);
		SKWOXEL_INLINE_SET_GET(real_t, altitude);
		SKWOXEL_INLINE_SET_GET(const godot::Vector3&, up);
		void balance_control_points();

	protected:
		void calculate_bounds(real_t max_start_radius);

	protected:
		godot::Ref<godot::Curve3D> curve;
		QuickCurve quick;
		godot::AABB bounds;
		real_t start_radius = 10.0;
		real_t end_radius = 10.0;
		real_t blend = 2.0;
		real_t inner_strength = 1.0;
		bool sliced = false;
		real_t outer_strength = -1.0;
		real_t altitude = 0.0;
		godot::Vector3 up = godot::Vector3(0.0, 1.0, 0.0);
	};
}

#endif
