/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include "register_types.h"

#include <godot/gdnative_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "skwoxel.h"
#include "skwoxel_field.h"
#include "skwoxel_field_sphere.h"

using namespace godot;

void initialize_skwoxel_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<skwoxel::Skwoxel>();
		ClassDB::register_class<skwoxel::SkwoxelField>();
		ClassDB::register_class<skwoxel::SkwoxelFieldSphere>();
	}
}

void uninitialize_skwoxel_module(ModuleInitializationLevel p_level) {
}

extern "C" {
// Initialization.
GDNativeBool GDN_EXPORT skwoxel_library_init(const GDNativeInterface *p_interface, const GDNativeExtensionClassLibraryPtr p_library, GDNativeInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_initializer(initialize_skwoxel_module);
	init_obj.register_terminator(uninitialize_skwoxel_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
