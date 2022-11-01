#include "skwoxel_field.h"

using namespace godot;

namespace skwoxel
{

	SkwoxelField::SkwoxelField() : Node(),
		child_fields(0)
	{

	}

	SkwoxelField::~SkwoxelField()
	{
		delete[] child_fields;
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
		return "[ wot? ]";
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

	void SkwoxelField::pre_generate()
	{
		for (int ch = 0; ch < num_child_fields; ch++)
		{
			child_fields[ch]->pre_generate();
		}
	}

	real_t SkwoxelField::strength(const Vector3& pos) const
	{
		return 0.0;
	}

	void SkwoxelField::post_generate()
	{
		for (int ch = 0; ch < num_child_fields; ch++)
		{
			child_fields[ch]->post_generate();
		}
	}

	void SkwoxelField::collect_children_of(const Node * parent)
	{
		if (child_fields)
		{
			delete[] child_fields;
			child_fields = 0;
		}
		int count = parent->get_child_count();
		int field_count = 0;
		for (int i = 0; i < count; i++)
		{
			Node* node = parent->get_child(i);
			SkwoxelField* child = dynamic_cast<SkwoxelField*>(node);
			if (child)
			{
				++field_count;
			}
		}

		// Why am I using my own array instead of a TypedArray?
		// Because TypedArray is really annoying! Yay!
		// Is this safe? NO! The children might cease to exist
		// and we will still have pointers to them.
		// This is very very internal. Do not expose to scripts.
		num_child_fields = field_count;
		child_fields = new SkwoxelField * [field_count];

		field_count = 0;
		for (int i = 0; i < count; i++)
		{
			Node* node = parent->get_child(i);
			SkwoxelField* child = dynamic_cast<SkwoxelField*>(node);
			if (child)
			{
				child_fields[field_count] = child;
				child->collect_children();
				++field_count;
			}
		}
	}

	void SkwoxelField::collect_children()
	{
		collect_children_of(this);
	}
}
