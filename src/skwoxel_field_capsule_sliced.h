#ifndef SKWOXEL_FIELD_CAPSULE_SLICED_H
#define SKWOXEL_FIELD_CAPSULE_SLICED_H

#include "skwoxel_field_capsule.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>

namespace skwoxel
{
	class SkwoxelFieldCapsuleSliced : public SkwoxelFieldCapsule
	{
		GDCLASS(SkwoxelFieldCapsuleSliced, SkwoxelFieldCapsule);
	public:
		SkwoxelFieldCapsuleSliced();
		virtual ~SkwoxelFieldCapsuleSliced();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		real_t strength(const godot::Vector3 & pos, const godot::Vector3& untransformed) const override;

		godot::Vector3 get_up() const { return up; };
		void set_up(godot::Vector3 p_up) { up = p_up; }
		real_t get_top_strength() const { return top_strength; };
		void set_top_strength(real_t p_top_strength) { top_strength = p_top_strength; }
		real_t get_altitude() const { return altitude; };
		void set_altitude(real_t p_altitude) { altitude = p_altitude; }

	private:
		godot::Vector3 up;
		real_t altitude;
		real_t top_strength;
	};
}

#endif
