#include "skwoxel_field_displace3.h"

#include "random.h"
#include "skwoxel_helpers.h"

using namespace godot;
using std::vector;

namespace skwoxel
{
	void SkwoxelFieldDisplace3::_notification(int p_what) {
	}

	bool SkwoxelFieldDisplace3::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(scale);
		SKWOXEL_SET_METHOD(magnitude_x);
		SKWOXEL_SET_METHOD(magnitude_y);
		SKWOXEL_SET_METHOD(magnitude_z);

		return false;
	}

	bool SkwoxelFieldDisplace3::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(scale);
		SKWOXEL_GET_METHOD(magnitude_x);
		SKWOXEL_GET_METHOD(magnitude_y);
		SKWOXEL_GET_METHOD(magnitude_z);

		return false;
	}

	String SkwoxelFieldDisplace3::_to_string() const {
		return "[Skwoxel field : DISPLACE3]";
	}

	void SkwoxelFieldDisplace3::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldDisplace3::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldDisplace3::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldDisplace3::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldDisplace3, scale);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldDisplace3, magnitude_x);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldDisplace3, magnitude_y);
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldDisplace3, magnitude_z);

		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, scale);
		SKWOXEL_ADD_PROPERTY(Variant::STRING, magnitude_x);
		SKWOXEL_ADD_PROPERTY(Variant::STRING, magnitude_y);
		SKWOXEL_ADD_PROPERTY(Variant::STRING, magnitude_z);
	}

	SkwoxelFieldDisplace3::SkwoxelFieldDisplace3() :
		SkwoxelFieldAdd(),
		scale(1.0, 1.0, 1.0)
	{
		magnitude[0] = "dispx";
		magnitude[1] = "dispy";
		magnitude[2] = "dispz";
	}

	SkwoxelFieldDisplace3::~SkwoxelFieldDisplace3()
	{

	}

	void SkwoxelFieldDisplace3::pre_generate(bool randomize_seeds, int num_threads)
	{
		int count = 0;
		for (int i = 0; i < 3; ++i)
		{
			displacement[i] = 0;
			if (magnitude[i].length() > 0)
			{
				Node* node = find_child(magnitude[i], false, false);
				displacement[i] = dynamic_cast<SkwoxelField*>(node);
				if (!displacement[i])
					UtilityFunctions::print("Displace 3 field has no child named [", magnitude[i], "]");
				else if (!displacement[i]->is_enabled())
					displacement[i] = 0;
			}
		}
		SkwoxelFieldAdd::pre_generate(randomize_seeds, num_threads);
	}

	real_t SkwoxelFieldDisplace3::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		Vector3 offset(0.0, 0.0, 0.0);
		for (int i = 0; i < 3; ++i)
		{
			if (displacement[i])
			{
				offset[i] = displacement[i]->strength(pos, untransformed, thread_num) * scale[i];
			}
		}
		Vector3 disp = pos - offset;
		real_t sum = 0.0;
		for (int ch = 0; ch < child_fields.size(); ch++)
		{
			SkwoxelField* child = child_fields[ch];
			if (child->is_enabled() && child != displacement[0] && child != displacement[1] && child != displacement[2])
				sum += child->strength(disp, untransformed, thread_num);
		}
		return sum;
	}
}
