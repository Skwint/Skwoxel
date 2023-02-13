# Skwoxel
A Small voxel GDExtension for [Godot 4](https://github.com/godotengine/godot)
![A skwoxel world](https://i.imgur.com/CzPdRMv.png)
![A procedural maze](https://i.imgur.com/SOtDmHp.png)

## Alternative voxels
There is already a far more sophisticated voxel system for Godot: [Zylann's Voxels](https://github.com/Zylann/godot_voxel)

Skwoxel is smaller and simpler with much less integration in the editor interface (so far, anyway)

* If you want infinite terrains, use Zylann's.
* If you want streaming tiles with dynamic LOD, use Zylann's.
* If you want background surface generation with multiple threads, use Zylann's.
* If you want to modify your terrain with explosions at run time, use Zylann's.
* If you only need one small area (such as a limited size level in a game) then you can use either.
* If you need to guarantee connectivity between locations, use Skwoxel.

The reason this exists is because dynamically loading tiles of voxels makes it extremely hard (impossible?) to remove disconnected floating rocks and isolated underground bubbles of air. Floating rocks are distracting and weird, and both floaters and bubbles present potential spawn points for monsters or items that cannot be pathed to, which is less than ideal if they spawn the all important key to the next dungeon, or the guy with the quest marker.

It also makes generating a navigation mesh more challenging. Much more challenging. Arguably impossible. It's horrible enough already.

This is only a major issue if you want a procedural world. If you are using voxels as a simple way to start a world you edit manually it isn't really a concern.

My gut feeling, after using this for a while, is that although it works exactly as intended and is, infact, kind of cool, it will never generate a world that has the kind of detail and polish we expect these days, so while I try to come up with a way to fix that the project is on hold. Not dead, just waiting for inspiration.

## Algorithm
Skwoxel uses Marching Tetrahedra to generate an isosurface of a field, a reasonably simple mesh simplification routine to keep the triangle count down, and uses the field gradients to calculate normals for a little extra smoothness in the end result. This can backfire if the field has components with a higher frequency than the voxels, but you should avoid that anyway, and it usually works out better than averaging triangle normals.

Dynamic editing of the voxels during gameplay is not really viable. You _can_ remove sections of it by adding a spherical field with a negative field strength, and that works just fine, but you will need to regenerate the entire mesh. It's not unreasonably slow, but it's not fast enough for anything that isn't turn based.

## Building
You need Godot-4. There is no support for earlier versions. Skwoxel requires the [godot-cpp](https://github.com/godotengine/godot-cpp) package and this is referenced as a submodule, but you may need to give git a bit of a kick to make it clone. Skwoxel expects to see godot-cpp/SConstruct at build time. Once that is in place you just need to run `scons target=template_debug` in the Skwoxel directory to build the extension for use in the editor.

## Usage
There is an example project in the 'test' subdirectory. To run it you will need to copy the shared library from bin to test/bin. Until GDextensions support help files, learning by example from the maze scene is probably the best option.

To use Skwoxel in a project the project must contain the skwoxel extension .dll / .so file, and the Skwoxel.gdextension file, which must be edited to contain the path to the library. Once that is done the editor will show `Skwoxel` as a specialization of StaticBody3D, and a variety of control objects as specializations of `SkwoxelField` under the `Node` tree.

Insert a Skwoxel into your scene and then add various fields to it as children in the scene tree. It is intended that this will be done by scripts, but the Skwoxel node contains a "generate" checkbox in the editor that can be used to cause the mesh to be generated and displayed. I really should automate that bit. At the moment your scene script will need to call generate() in the _ready() function.

## Development status
This software is in alpha, which means you can use it, but it throws up a few error messages, isn't as well integrated into the editor as it should be, and just generally isn't really polished enough to call a beta. It definitely will not become a beta until godot-4 is stable and godot-cpp is at least beta itself. You are welcome to play with it anyway if you like. I can usually be found in the Godot discord server if you have questions.
