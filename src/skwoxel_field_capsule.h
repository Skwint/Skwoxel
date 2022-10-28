#ifndef SKWOXEL_FIELD_CAPSULE_H
#define SKWOXEL_FIELD_CAPSULE_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>

namespace skwoxel
{
	class SkwoxelFieldCapsule : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldCapsule, SkwoxelField);
	public:
		SkwoxelFieldCapsule();
		virtual ~SkwoxelFieldCapsule();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		real_t strength(const godot::Vector3 & pos) override;

		godot::Vector3 get_point1() const { return point1; }
		void set_point1(const godot::Vector3& point) { point1 = point; }
		godot::Vector3 get_point2() const { return point2; }
		void set_point2(const godot::Vector3& point) { point2 = point; }
		real_t get_inner_radius() const { return inner_radius; };
		void set_inner_radius(real_t radius) { inner_radius = radius; }
		real_t get_outer_radius() const { return outer_radius; };
		void set_outer_radius(real_t radius) { outer_radius = radius; }
		real_t get_inner_strength() const { return inner_strength; };
		void set_inner_strength(real_t strength) { inner_strength = strength; }
		real_t get_outer_strength() const { return outer_strength; };
		void set_outer_strength(real_t strength) { outer_strength = strength; }

	private:
		godot::Vector3 point1;
		godot::Vector3 point2;
		real_t inner_radius;
		real_t outer_radius;
		real_t inner_strength;
		real_t outer_strength;
	};
}

#endif
