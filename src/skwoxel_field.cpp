#include "skwoxel_field.h"

using namespace godot;
using std::vector;

namespace skwoxel
{

	SkwoxelField::SkwoxelField() : Node()
	{

	}

	SkwoxelField::~SkwoxelField()
	{
	}

	void SkwoxelField::_notification(int p_what) {
	}

	bool SkwoxelField::_set(const StringName& p_name, const Variant& p_value) {
		return false;
	}

	bool SkwoxelField::_get(const StringName& p_name, Variant& r_ret) const {
		return false;
	}

	String SkwoxelField::_to_string() const {
		return "[Skwoxel field : ? field ?]";
	}

	void SkwoxelField::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelField::_property_can_revert(const StringName& p_name) const {
		return true;
	}

	bool SkwoxelField::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelField::_bind_methods()
	{
	}

	void SkwoxelField::pre_generate(bool randomize_seeds, int num_threads)
	{
		for (int ch = 0; ch < child_fields.size(); ch++)
		{
			child_fields[ch]->pre_generate(randomize_seeds, num_threads);
		}
	}

	real_t SkwoxelField::strength(const Vector3& pos, const Vector3& untransformed, int thread_num) const
	{
		return 0.0;
	}

	void SkwoxelField::post_generate(vector<Vector3>& air_points, vector<Vector3>& ground_points)
	{
		for (int ch = 0; ch < child_fields.size(); ch++)
		{
			child_fields[ch]->post_generate(air_points, ground_points);
		}
	}

	void SkwoxelField::collect_children_of(const Node * parent)
	{
		child_fields.clear();
		int count = parent->get_child_count();
		child_fields.reserve(count);

		for (int i = 0; i < count; i++)
		{
			Node* node = parent->get_child(i);
			SkwoxelField* child = dynamic_cast<SkwoxelField*>(node);
			//SkwoxelField* child = godot::Object::cast_to<SkwoxelField>(node);
			if (child)
			{
				child_fields.push_back(child);
				child->collect_children();
			}
		}
	}

	void SkwoxelField::collect_children()
	{
		collect_children_of(this);
	}
}
