#ifndef SKWOXEL_FIELD_DISPLACE_H
#define SKWOXEL_FIELD_DISPLACE_H

#include "skwoxel_field_add.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace skwoxel
{
	class SkwoxelFieldDisplace : public SkwoxelFieldAdd
	{
		GDCLASS(SkwoxelFieldDisplace, SkwoxelFieldAdd);
	public:
		SkwoxelFieldDisplace();
		virtual ~SkwoxelFieldDisplace();

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
		godot::Vector3 get_direction() const { return direction; }
		void set_direction(godot::Vector3 p_direction) { direction = p_direction; }
		godot::String get_magnitude() const { return magnitude; }
		void set_magnitude(const godot::String& p_magnitude) { magnitude = p_magnitude; }

	private:
		godot::Vector3 direction;
		godot::String magnitude;
		SkwoxelField* displacement;
	};
}

#endif
