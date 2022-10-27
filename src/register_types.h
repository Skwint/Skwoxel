/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#ifndef SKWOXEL_REGISTER_TYPES_H
#define SKWOXEL_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_skwoxel_module(ModuleInitializationLevel p_level);
void uninitialize_skwoxel_module(ModuleInitializationLevel p_level);

#endif // SKWOXEL_REGISTER_TYPES_H
