#ifndef SKWOXEL_INSPECTOR_PLUGIN_H
#define SKWOXEL_INSPECTOR_PLUGIN_H

#include <godot_cpp/classes/editor_inspector_plugin.hpp>

namespace skwoxel
{

	class SkwoxelInspectorPlugin : public godot::EditorInspectorPlugin {
		GDCLASS(SkwoxelInspectorPlugin, godot::EditorInspectorPlugin)
	public:
		bool _can_handle(const godot::Variant& p_object_v) const override;
		void _parse_begin(godot::Object* p_object) override;

	private:
		// When compiling with GodotCpp, `_bind_methods` is not optional.
		static void _bind_methods() {}
	};
}

#endif
