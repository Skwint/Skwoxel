#include "skwoxel_field_displace.h"

#include "random.h"
#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldDisplace::_notification(int p_what) {
	}

	bool SkwoxelFieldDisplace::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(direction);
		SKWOXEL_SET_METHOD(magnitude);

		return false;
	}

	bool SkwoxelFieldDisplace::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(direction);
		SKWOXEL_GET_METHOD(magnitude);

		return false;
	}

	String SkwoxelFieldDisplace::_to_string() const {
		return "[Skwoxel field : DISPLACE]";
	}

	void SkwoxelFieldDisplace::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldDisplace::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldDisplace::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldDisplace::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldDisplace, direction);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldDisplace, magnitude);

		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, direction);
		SKWOXEL_ADD_PROPERTY(Variant::STRING, magnitude);
	}

	SkwoxelFieldDisplace::SkwoxelFieldDisplace() :
		SkwoxelFieldAdd(),
		direction(0.0, 0.0, 0.0),
		magnitude("displacement")
	{

	}

	SkwoxelFieldDisplace::~SkwoxelFieldDisplace()
	{

	}

	void SkwoxelFieldDisplace::pre_generate(bool randomize_seeds)
	{
		Node* node = find_child(magnitude, false, false);
		displacement = dynamic_cast<SkwoxelField*>(node);
		if (!displacement)
		{
			UtilityFunctions::print("Displace field has no child named [", magnitude, "]");
		}
		SkwoxelFieldAdd::pre_generate(randomize_seeds);
	}

	void SkwoxelFieldDisplace::trigger(const Vector3& pos, const Vector3& untransformed)
	{
		real_t offset = 0.0;
		if (displacement)
		{
			offset = displacement->strength(pos);
		}
		Vector3 disp = pos - offset * direction;
		for (int ch = 0; ch < child_fields.size(); ch++)
		{
			SkwoxelField* child = child_fields[ch];
			if (child != displacement)
				child->trigger(disp, untransformed);
		}
	}

	real_t SkwoxelFieldDisplace::strength(const Vector3& pos) const
	{
		real_t offset = 0.0;
		if (displacement)
		{
			offset = displacement->strength(pos);
		}
		Vector3 disp = pos - offset * direction;
		real_t sum = 0.0;
		for (int ch = 0; ch < child_fields.size(); ch++)
		{
			SkwoxelField* child = child_fields[ch];
			if (child != displacement)
				sum += child->strength(disp);
		}
		return sum;
	}
}
