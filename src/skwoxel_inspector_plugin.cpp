#include "skwoxel_inspector_plugin.h"

#include "skwoxel.h"
#include "skwoxel_inspector_controls.h"

using namespace godot;

namespace skwoxel
{
	bool SkwoxelInspectorPlugin::_can_handle(const Variant & p_object_v) const
	{
		const Object* p_object = p_object_v;
		return Object::cast_to<Skwoxel>(p_object) != nullptr;
	}

	void SkwoxelInspectorPlugin::_parse_begin(Object * p_object)
	{
		Skwoxel * skwoxel = Object::cast_to<Skwoxel>(p_object);
		ERR_FAIL_COND(skwoxel == nullptr);
		SkwoxelInspectorControls* ctrls = memnew(SkwoxelInspectorControls);
		add_custom_control(ctrls);
		ctrls->set_skwoxel(skwoxel);
	}
}
