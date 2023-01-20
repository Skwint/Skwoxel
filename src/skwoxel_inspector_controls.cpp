#include "skwoxel_inspector_controls.h"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	SkwoxelInspectorControls::SkwoxelInspectorControls()
	{
		bake_button = memnew(Button);
		bake_button->connect("pressed", SKWOXEL_CALLABLE(this, "on_bake_button_pressed"));
		add_child(bake_button);
	}

	void SkwoxelInspectorControls::on_bake_button_pressed()
	{
	}

	void SkwoxelInspectorControls::_bind_methods() {
		ClassDB::bind_method(D_METHOD("on_bake_button_pressed"), &SkwoxelInspectorControls::on_bake_button_pressed);
	}

}

