#ifndef SKWOXEL_FIELD_CYLINDER_H
#define SKWOXEL_FIELD_CYLINDER_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>

namespace skwoxel
{
	class SkwoxelFieldCylinder : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldCylinder, SkwoxelField);
	public:
		SkwoxelFieldCylinder();
		virtual ~SkwoxelFieldCylinder();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		void pre_generate(bool randomize_seeds, int num_threads);
		real_t strength(const godot::Vector3& pos, const godot::Vector3& untransformed, int thread_num) const override;

		godot::Vector3 get_point1() const { return point1; }
		void set_point1(const godot::Vector3& point) { point1 = point; }
		godot::Vector3 get_point2() const { return point2; }
		void set_point2(const godot::Vector3& point) { point2 = point; }
		real_t get_radius() const { return radius; };
		void set_radius(real_t p_radius) { radius = p_radius; }
		real_t get_blend() const { return blend; };
		void set_blend(real_t p_blend) { blend = MAX(0.1, p_blend); }
		real_t get_inner_strength() const { return inner_strength; };
		void set_inner_strength(real_t p_strength) { inner_strength = p_strength; }

	private:
		godot::Vector3 point1;
		godot::Vector3 point2;
		real_t radius;
		real_t blend;
		real_t inner_strength;
		real_t length;
		godot::Vector3 normal;
	};
}

#endif
