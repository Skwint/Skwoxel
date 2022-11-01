#ifndef SKWOXEL_FIELD_IMAGE_CURVE_H
#define SKWOXEL_FIELD_IMAGE_CURVE_H

#include "skwoxel_field.h"

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/curve3d.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/variant/typed_array.hpp>

namespace skwoxel
{
	class SkwoxelFieldImageCurve : public SkwoxelField
	{
		GDCLASS(SkwoxelFieldImageCurve, SkwoxelField);
	public:
		SkwoxelFieldImageCurve();
		virtual ~SkwoxelFieldImageCurve();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		void pre_generate() override;
		real_t strength(const godot::Vector3 & pos) const override;
		void post_generate() override;

		godot::Ref<godot::Curve3D> get_curve() const { return curve; };
		godot::Ref<godot::Texture2D> get_texture() const;
		void set_texture(const godot::Ref<godot::Texture2D> tex);
		void set_curve(const godot::Ref<godot::Curve3D> cur) { curve = cur; }
		real_t get_texture_scale() const { return texture_scale; };
		void set_texture_scale(real_t radius) { texture_scale = radius; }
		real_t get_black_strength() const { return black_strength; };
		void set_black_strength(real_t radius) { black_strength = radius; }
		real_t get_white_strength() const { return white_strength; };
		void set_white_strength(real_t strength) { white_strength = strength; }

	private:
		godot::Ref<godot::Curve3D> curve;
		godot::Ref<godot::Texture2D> texture;
		godot::Ref<godot::Image> image;
		godot::PackedVector3Array points;
		godot::PackedVector3Array ups;
		godot::PackedVector3Array rights;
		real_t texture_scale;
		real_t black_strength;
		real_t white_strength;
	};
}

#endif
