/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include "skwoxel.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "skwoxel_helpers.h"
#include "skwoxel_field.h"

using namespace godot;

namespace skwoxel
{
	void Skwoxel::_notification(int p_what) {
	}

	bool Skwoxel::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(lower_bounds);
		SKWOXEL_SET_METHOD(upper_bounds);
		return false;
	}

	bool Skwoxel::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(lower_bounds);
		SKWOXEL_GET_METHOD(upper_bounds);
		return false;
	}

	String Skwoxel::_to_string() const {
		return "[ GDExtension::Skwoxel <--> Instance ID:" + itos(get_instance_id()) + " ]";
	}

	void Skwoxel::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::VECTOR3I, "lower_bounds"));
		list->push_back(PropertyInfo(Variant::VECTOR3I, "upper_bounds"));
	}

	bool Skwoxel::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool Skwoxel::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void Skwoxel::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, lower_bounds);
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, upper_bounds);
		ClassDB::bind_method(D_METHOD("clear_fields"), &Skwoxel::clear_fields);
		ClassDB::bind_method(D_METHOD("generate_fields"), &Skwoxel::generate_fields);
	}

	Skwoxel::Skwoxel() :
		field_strengths(0)
	{
	}

	Skwoxel::~Skwoxel()
	{
		delete_fields();
	}

	void Skwoxel::allocate_fields()
	{
		if (!field_strengths)
		{
			field_strengths = new float[size_x() * size_y() * size_x()];
		}
	}

	void Skwoxel::delete_fields()
	{
		delete[] field_strengths;
		field_strengths = 0;
	}

	void Skwoxel::clear_fields()
	{
		float* voxel = field_strengths;
		for (int z = lower_bounds.z; z <= upper_bounds.z; ++z)
		{
			for (int y = lower_bounds.y; y <= upper_bounds.y; ++y)
			{
				for (int x = lower_bounds.x; x <= upper_bounds.x; ++x)
				{
					*voxel++ = 0.0;
				}
			}
		}
	}

	void Skwoxel::generate_fields()
	{
		allocate_fields();

		clear_fields();

		int count = get_child_count();
		for (int i = 0; i < count; i++)
		{
			Node* node = get_child(i);
			SkwoxelField * child = dynamic_cast<SkwoxelField*>(node);
			if (child)
			{
				float* voxel = field_strengths;
				for (int z = lower_bounds.z; z <= upper_bounds.z; ++z)
				{
					for (int y = lower_bounds.y; y <= upper_bounds.y; ++y)
					{
						for (int x = lower_bounds.x; x <= upper_bounds.x; ++x)
						{
							*voxel += child->strength(Vector3(x, y, z));
							++voxel;
						}
					}
				}
			}
		}
	}

	// Properties.
	void Skwoxel::set_lower_bounds(const Vector3i& bounds) {
		lower_bounds = bounds;
		delete_fields();
	}

	Vector3i Skwoxel::get_lower_bounds() const {
		return lower_bounds;
	}

	void Skwoxel::set_upper_bounds(const Vector3i& bounds) {
		upper_bounds = bounds;
		delete_fields();
	}

	Vector3i Skwoxel::get_upper_bounds() const {
		return upper_bounds;
	}
}
