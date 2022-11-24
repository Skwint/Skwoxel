#include "skwoxel_field_wiggle.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldWiggle::_notification(int p_what) {
	}

	bool SkwoxelFieldWiggle::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(end_radius);

		return false;
	}

	bool SkwoxelFieldWiggle::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(end_radius);

		return false;
	}

	String SkwoxelFieldWiggle::_to_string() const {
		return "[Skwoxel field : WIGGLE]";
	}

	bool SkwoxelFieldWiggle::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldWiggle::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldWiggle::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldWiggle, end_radius);

		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, end_radius);
	}

	SkwoxelFieldWiggle::SkwoxelFieldWiggle() :
		SkwoxelFieldCurve(),
		end_radius(10.0)
	{

	}

	SkwoxelFieldWiggle::~SkwoxelFieldWiggle()
	{

	}

	void SkwoxelFieldWiggle::pre_generate(bool randomize_seeds, int num_threads)
	{
		// Take care we are skipping the SkwoxelFieldCurve version of this function here
		SkwoxelField::pre_generate(randomize_seeds, num_threads);
		calculate_bounds(MAX(radius, end_radius));
	}

	real_t SkwoxelFieldWiggle::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		if (curve.is_valid() && bounds.has_point(pos))
		{
			real_t offset = curve->get_closest_offset(pos);
			real_t length = curve->get_baked_length();
			Vector3 touch = curve->sample_baked(offset);
			real_t rad = (pos - touch).length();
			real_t interpolated_radius = radius + (offset / length) * (end_radius - radius);
			real_t radial_multiplier = smooth_step(-blend, blend, interpolated_radius - rad);
			return inner_strength * radial_multiplier;
		}
		else
		{
			return 0.0;
		}
	}
}
