# Skwoxel
A Small voxel GDExtension for [Godot 4](https://github.com/godotengine/godot)

## Alternative voxels
There is already a far more sophisticated voxel system for Godot, at [Zylann's Voxels](https://github.com/Zylann/godot_voxel)

Skwoxel is smaller and simpler with much less integration in the editor interface (so far, anyway)

* If you want infinite terrains, use Zylann's.
* If you want streaming tiles with dynamic LOD, use Zylann's.
* If you want to modify your terrain with explosions at run time, *probably* use Zylann's.
* If you only need one small area (such as a limited size level in a game) then you can use either.
* If you need to guarantee connectivity between locations, use Skwoxel.

The reason this exists is because dynamically loading tiles of voxels makes it extremely hard (impossible?) to remove disconnected floating rocks and isolated underground bubbles of air. Floating rocks are distracting and weird, and both floaters and bubbles present potential spawn points for monsters or items that cannot be pathed to, which is less than ideal if they spawn the all important key to the next dungeon, or the guy with the quest marker.

It also makes generating a navigation mesh somewhat challenging.

## Algorithm
Skwoxel uses Marching Tetrahedra to generate an isosurface of a field, and uses the field gradients to calculate normals for a little extra smoothness in the end result.

## Building
Skwoxel requires the [godot-cpp](https://github.com/godotengine/godot-cpp) package, and expects it to be at the same level in the filesystem (Skwoxel expects to see ../godot-cpp/SConstruct). Once that is done you just need to run `scons target=template_debug` in the Skwoxel directory to build the extension for use in the editor.

## Usage
To use Skwoxel in a project the project must contain the skwoxel extension .dll / .so file, and the Skwoxel.gdextension file, which must be edited to contain the path to the library. Once that is done the editor will show `Skwoxel` as a specialization of StaticBody3D, and a variety of control objects as specializations of `SkwoxelField` under the `Node` tree.

Insert a Skwoxel into your scene and then add various fields to it as children in the scene tree. The Skwoxel node contains a "generate" checkbox that doesn't actually check, it just causes the mesh to be generated and displayed in the editor. I really should automate that bit.

## Development status
This software is not yet alpha (it is still way too incomplete and buggy to be called an alpha) but you are welcome to play with it anyway if you like. I can usually be found in the Godot discord server if you have questions.
