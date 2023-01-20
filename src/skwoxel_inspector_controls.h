#ifndef SKWOXEL_INSPECTOR_CONTROLS_H
#define SKWOXEL_INSPECTOR_CONTROLS_H

#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/button.hpp>

namespace skwoxel
{
	class Skwoxel;

	class SkwoxelInspectorControls : public godot::VBoxContainer
	{
		GDCLASS(SkwoxelInspectorControls, godot::VBoxContainer)
	public:
		SkwoxelInspectorControls();

		void set_skwoxel(Skwoxel* p_skwoxel) { skwoxel = p_skwoxel; }

	private:
		void on_bake_button_pressed();

		static void _bind_methods();

	private:
		godot::Button* bake_button = nullptr;
		Skwoxel* skwoxel = nullptr;
	};

}

#endif
