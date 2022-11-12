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
#include "skwoxel_field_add.h"
#include "skwoxel_field_capsule.h"
#include "skwoxel_field_constant.h"
#include "skwoxel_field_curve.h"
#include "skwoxel_field_cut.h"
#include "skwoxel_field_cylinder.h"
#include "skwoxel_field_image_curve.h"
#include "skwoxel_field_mirror.h"
#include "skwoxel_field_multiply.h"
#include "skwoxel_field_noise.h"
#include "skwoxel_field_plane.h"
#include "skwoxel_field_rotate.h"
#include "skwoxel_field_scale.h"
#include "skwoxel_field_select.h"
#include "skwoxel_field_sphere.h"
#include "skwoxel_field_torus.h"
#include "skwoxel_field_translate.h"
#include "skwoxel_field_turbulence.h"
#include "skwoxel_field_walkway.h"
#include "skwoxel_field_wiggle.h"

using namespace godot;

void initialize_skwoxel_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<skwoxel::Skwoxel>();
		ClassDB::register_class<skwoxel::SkwoxelField>();
		ClassDB::register_class<skwoxel::SkwoxelFieldAdd>();
		ClassDB::register_class<skwoxel::SkwoxelFieldCapsule>();
		ClassDB::register_class<skwoxel::SkwoxelFieldConstant>();
		ClassDB::register_class<skwoxel::SkwoxelFieldCurve>();
		ClassDB::register_class<skwoxel::SkwoxelFieldCut>();
		ClassDB::register_class<skwoxel::SkwoxelFieldCylinder>();
		//ClassDB::register_class<skwoxel::SkwoxelFieldImageCurve>();
		ClassDB::register_class<skwoxel::SkwoxelFieldMirror>();
		ClassDB::register_class<skwoxel::SkwoxelFieldMultiply>();
		ClassDB::register_class<skwoxel::SkwoxelFieldNoise>();
		ClassDB::register_class<skwoxel::SkwoxelFieldPlane>();
		ClassDB::register_class<skwoxel::SkwoxelFieldRotate>();
		ClassDB::register_class<skwoxel::SkwoxelFieldScale>();
		ClassDB::register_class<skwoxel::SkwoxelFieldSelect>();
		ClassDB::register_class<skwoxel::SkwoxelFieldSphere>();
		ClassDB::register_class<skwoxel::SkwoxelFieldTorus>();
		ClassDB::register_class<skwoxel::SkwoxelFieldTranslate>();
		ClassDB::register_class<skwoxel::SkwoxelFieldTurbulence>();
		ClassDB::register_class<skwoxel::SkwoxelFieldWalkway>();
		ClassDB::register_class<skwoxel::SkwoxelFieldWiggle>();
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
