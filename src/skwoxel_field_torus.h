#ifndef SKWOXEL_FIELD_TORUS_H
#define SKWOXEL_FIELD_TORUS_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>

namespace skwoxel
{
	class SkwoxelFieldTorus : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldTorus, SkwoxelField);
	public:
		SkwoxelFieldTorus();
		virtual ~SkwoxelFieldTorus();

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

		godot::Vector3 get_centre() const { return centre; }
		void set_centre(const godot::Vector3& p_centre) { centre = p_centre; }
		godot::Vector3 get_axis() const { return axis; }
		void set_axis(const godot::Vector3& p_axis) { axis = p_axis; }
		real_t get_major_radius() const { return major_radius; };
		void set_major_radius(real_t p_radius) { major_radius = p_radius; }
		real_t get_minor_radius() const { return minor_radius; };
		void set_minor_radius(real_t p_radius) { minor_radius = p_radius; }
		real_t get_blend() const { return blend; };
		void set_blend(real_t p_blend) { blend = std::max((real_t)0.1, p_blend); }
		real_t get_inner_strength() const { return inner_strength; };
		void set_inner_strength(real_t p_strength) { inner_strength = p_strength; }

	private:
		godot::Vector3 centre;
		godot::Vector3 axis;
		real_t major_radius;
		real_t minor_radius;
		real_t blend;
		real_t inner_strength;
	};
}

#endif
