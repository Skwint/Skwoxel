#include "skwoxel_field_image_curve.h"

#include <godot_cpp/classes/geometry3d.hpp>

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldImageCurve::_notification(int p_what) {
	}

	bool SkwoxelFieldImageCurve::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(curve);
		SKWOXEL_SET_METHOD(texture);
		SKWOXEL_SET_METHOD(texture_scale);
		SKWOXEL_SET_METHOD(black_strength);
		SKWOXEL_SET_METHOD(white_strength);

		return false;
	}

	bool SkwoxelFieldImageCurve::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(curve);
		SKWOXEL_GET_METHOD(texture);
		SKWOXEL_GET_METHOD(texture_scale);
		SKWOXEL_GET_METHOD(black_strength);
		SKWOXEL_GET_METHOD(white_strength);

		return false;
	}

	String SkwoxelFieldImageCurve::_to_string() const {
		return "[Skwoxel field : IMAGE CURVE]";
	}

	void SkwoxelFieldImageCurve::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldImageCurve::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldImageCurve::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldImageCurve::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldImageCurve, curve);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldImageCurve, texture);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldImageCurve, texture_scale);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldImageCurve, black_strength);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldImageCurve, white_strength);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve3D"), "set_curve", "get_curve");
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_texture", "get_texture");
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, texture_scale);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, black_strength);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, white_strength);
	}

	SkwoxelFieldImageCurve::SkwoxelFieldImageCurve() :
		SkwoxelField(),
		texture_scale(1.0),
		black_strength(-1.0),
		white_strength(1.0)
	{

	}

	SkwoxelFieldImageCurve::~SkwoxelFieldImageCurve()
	{

	}

	void SkwoxelFieldImageCurve::pre_generate(bool randomize_seeds)
	{
		real_t interval = curve->get_bake_interval();
		for (int idx = 0; idx < curve->get_point_count(); ++idx)
		{
			for (real_t offset = 0.0; offset < 1.0; offset += interval)
			{
				Vector3 point = curve->sample(idx, offset);
				points.push_back(point);
			}
		}
		Vector3 xaxis(1.0, 0.0, 0.0);
		Vector3 zaxis(0.0, 0.0, 1.0);
		Vector3 up;
		Vector3 right;
		for (int idx = 0; idx < points.size() - 1; ++idx)
		{
			Vector3 point = points[idx];
			Vector3 next = points[idx + 1];
			Vector3 dir = next - point;
			if (dir.x > dir.z)
			{
				up = Vector3(dir.x, 0.0, 0.0).cross(zaxis);
			}
			else
			{
				up = xaxis.cross(Vector3(0.0, 0.0, dir.z));
			}
			if (up.y < 0.0)
			{
				up = -up;
			}
			up.normalize();
			right = up.cross(dir);
			right.normalize();
			ups.push_back(up);
			rights.push_back(right);
		}
		ups.push_back(up);
		rights.push_back(right);
	}

	real_t SkwoxelFieldImageCurve::strength(const Vector3& pos) const
	{
		if (image.is_valid() && curve.is_valid())
		{
			Vector3 closest;
			real_t closest_distance = (image->get_width() * image->get_width() + image->get_height() * image->get_height());
			int closest_idx = -1;
			for (int idx = 0; idx < points.size() - 1; ++idx)
			{
				Vector3 point1 = points[idx];
				Vector3 point2 = points[idx + 1];
				Vector3 close = Geometry3D::get_singleton()->get_closest_point_to_segment(pos, point1, point2);
				auto distance_squared = (pos - close).length_squared();
				if (distance_squared < closest_distance)
				{
					closest = close;
					closest_distance = distance_squared;
					closest_idx = idx;
				}
			}
			if (closest_idx < 0)
			{
				return 0.0;
			}
			//int x = (pos - closest).dot(rights[closest_idx]) + float(image->get_width()) * 0.5;
			//int y = (pos - closest).dot(ups[closest_idx]) + float(image->get_height()) * 0.5;
			int x = (pos - closest).x + float(image->get_width()) * 0.5;
			int y = (pos - closest).y + float(image->get_height()) * 0.5;
			if (x < 0 || x >= image->get_width() || y < 0 || y >= image->get_height())
			{
				return 0.0;
			}
			return black_strength + image->get_pixel(x, y).get_v() * (white_strength - black_strength);
		}
		else
		{
			return 0.0;
		}
	}

	void SkwoxelFieldImageCurve::post_generate()
	{
		points.clear();
		ups.clear();
		rights.clear();
	}

	Ref<Texture2D> SkwoxelFieldImageCurve::get_texture() const
	{
		return texture;
	}

	void SkwoxelFieldImageCurve::set_texture(const Ref<Texture2D> tex)
	{
		texture = tex;
		if (texture.is_valid())
		{
			image = texture->get_image();
		}
	}
}
