#ifndef SKWOXEL_FIELD_H
#define SKWOXEL_FIELD_H

#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/wrapped.hpp>

using godot::StringName; // This is bad, but GDCLASS is being unfriendly

namespace skwoxel
{
	class SkwoxelField : public godot::Node
	{
		GDCLASS(SkwoxelField, godot::Node);
	public:
		SkwoxelField();
		virtual ~SkwoxelField();

		bool _set(const godot::StringName& p_name, const godot::Variant& p_value);
		bool _get(const godot::StringName& p_name, godot::Variant& r_ret) const;
		void _get_property_list(godot::List<godot::PropertyInfo>* p_list) const;
		bool _property_can_revert(const godot::StringName& p_name) const;
		bool _property_get_revert(const godot::StringName& p_name, godot::Variant& r_property) const;
		godot::String _to_string() const;
		void _notification(int p_what);
		static void _bind_methods();

		virtual void pre_generate(bool randomize_seeds);
		virtual real_t strength(const godot::Vector3 & pos) const;
		virtual void post_generate();
		void collect_children();
		void collect_children_of(const godot::Node* parent);

	protected:
		int num_child_fields;
		SkwoxelField** child_fields;
	};
}

#endif
