#ifndef SKWOXEL_FIELD_DISPLACE3_H
#define SKWOXEL_FIELD_DISPLACE3_H

#include "skwoxel_field_add.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/ref.hpp>

namespace skwoxel
{
	class SkwoxelFieldDisplace3 : public SkwoxelFieldAdd
	{
		GDCLASS(SkwoxelFieldDisplace3, SkwoxelFieldAdd);
	public:
		SkwoxelFieldDisplace3();
		virtual ~SkwoxelFieldDisplace3();

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
		godot::Vector3 get_scale() const { return scale; }
		void set_scale(godot::Vector3 p_scale) { scale = p_scale; }
		godot::String get_magnitude_x() const { return magnitude[0]; }
		void set_magnitude_x(const godot::String& p_magnitude_x) { magnitude[0] = p_magnitude_x; }
		godot::String get_magnitude_y() const { return magnitude[1]; }
		void set_magnitude_y(const godot::String& p_magnitude_y) { magnitude[1] = p_magnitude_y; }
		godot::String get_magnitude_z() const { return magnitude[2]; }
		void set_magnitude_z(const godot::String& p_magnitude_z) { magnitude[2] = p_magnitude_z; }

	private:
		godot::Vector3 scale;
		godot::String magnitude[3];
		SkwoxelField* displacement[3];
	};
}

#endif
