/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#ifndef SKWOXEL_SKWOXEL_H
#define SKWOXEL_SKWOXEL_H

#include <chrono>
#include <vector>
#include <godot_cpp/classes/static_body3d.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#include <godot_cpp/core/binder_common.hpp>

#include "lotsa.h"
#include "skwoxel_field.h"
#include "skwoxel_field_add.h"

using namespace godot;

namespace skwoxel
{
	class Skwoxel : public StaticBody3D {
		GDCLASS(Skwoxel, StaticBody3D);

	public:
		struct Voxel
		{
		public:
			enum Flag
			{
				GROUND = 1 << 0,
				AIR    = 1 << 1
			};
			enum EdgeNum
			{
				EEN_X,
				EEN_Y,
				EEN_Z,
				EEN_XY,
				EEN_XZ,
				EEN_YZ,
				EEN_XYZ,
				EEN_COUNT
			};
		public:
			real_t strength;
			int32_t edges[EEN_COUNT];
			uint32_t flags;
		};
	protected:
		static void _bind_methods();

		void _notification(int p_what);
		bool _set(const StringName& p_name, const Variant& p_value);
		bool _get(const StringName& p_name, Variant& r_ret) const;
		void _get_property_list(List<PropertyInfo>* p_list) const;
		bool _property_can_revert(const StringName& p_name) const;
		bool _property_get_revert(const StringName& p_name, Variant& r_property) const;

		String _to_string() const;

		real_t sample(const Vector3& pos, int thread_num = 0) const;
		void collect_children();
		void allocate_voxels();
		void delete_voxels();
		void generate_voxels();
		void generate_voxels_thread(int lowz, int highz, int thread_num);
		void delete_mesh();
		void generate_mesh();
		void generate_air_flags();
		void generate_ground_flags();
		void filter();
		inline int size_x() { return upper_bounds.x + 1 - lower_bounds.x; }
		inline int size_y() { return upper_bounds.y + 1 - lower_bounds.y; }
		inline int size_z() { return upper_bounds.z + 1 - lower_bounds.z; }
		inline int data_size_x() { return size_x() + 1; }
		inline int data_size_y() { return size_y() + 1; }
		inline int data_size_z() { return size_z() + 1; }
		inline Voxel& voxel_at_local_unsafe(int x, int y, int z);
		inline Voxel& voxel_at_global_unsafe(int x, int y, int z);
		inline Voxel& voxel_at_local(int x, int y, int z);
		inline Voxel& voxel_at_global(int x, int y, int z);

	private:
		void report(const godot::String& str);

	private:
		godot::Vector3i lower_bounds;
		godot::Vector3i upper_bounds;
		std::vector<godot::Vector3> air_points;
		std::vector<godot::Vector3> ground_points;
		bool remove_bubbles;
		bool remove_floaters;
		bool simplify_mesh;
		bool simple_normals;
		bool smooth_normals;
		real_t minimum_edge;
		bool randomize_seeds;
		int num_threads = 4;
		Ref<Material> material;
		SkwoxelFieldAdd root;
		lotsa<Voxel> voxels;
		std::chrono::time_point<std::chrono::steady_clock> start_time;
		std::chrono::time_point<std::chrono::steady_clock> last_time;

	public:
		Skwoxel();
		~Skwoxel();

		void generate();
		void filter_bubbles();
		void filter_floaters();

		// Property.
		void set_lower_bounds(const Vector3i& bounds);
		Vector3i get_lower_bounds() const;
		void set_upper_bounds(const Vector3i& bounds);
		Vector3i get_upper_bounds() const;
		void set_material(godot::Ref<godot::Material> p_material) { material = p_material; }
		godot::Ref<godot::Material> get_material() const { return material; }
		void set_remove_bubbles(bool remove);
		bool get_remove_bubbles() const;
		void set_remove_floaters(bool remove);
		bool get_remove_floaters() const;
		void set_simple_normals(bool p_simple_normals) { simple_normals = p_simple_normals; if (!simple_normals) set_smooth_normals(true); }
		bool get_simple_normals() const { return simple_normals; }
		void set_smooth_normals(bool p_smooth_normals) { smooth_normals = p_smooth_normals; if (!smooth_normals) set_simple_normals(true); }
		bool get_smooth_normals() const { return smooth_normals; }
		void set_minimum_edge(real_t p_minimum_edge) { minimum_edge = p_minimum_edge; }
		real_t get_minimum_edge() const { return minimum_edge; }
		void set_randomize_seeds(bool p_randomize_seeds) { randomize_seeds = p_randomize_seeds; }
		bool get_randomize_seeds() const { return randomize_seeds; }
		void set_num_threads(int p_num_threads) { num_threads = p_num_threads; }
		int get_num_threads() const { return num_threads; }

		// Ungodly editor hack
		void set_generate(bool val) { generate(); }
		bool get_generate() const { return false; }
	};

	inline Skwoxel::Voxel& Skwoxel::voxel_at_local_unsafe(int x, int y, int z)
	{
		return voxels[(z* data_size_y() + y) * data_size_x() + x];
	}

	inline Skwoxel::Voxel& Skwoxel::voxel_at_global_unsafe(int x, int y, int z)
	{
		return voxel_at_local_unsafe(x - lower_bounds.x, y - lower_bounds.y, z - lower_bounds.z);
	}

	inline Skwoxel::Voxel& Skwoxel::voxel_at_local(int x, int y, int z)
	{
		x = godot::Math::clamp(x, 0, size_x() - 1);
		y = godot::Math::clamp(y, 0, size_y() - 1);
		z = godot::Math::clamp(z, 0, size_z() - 1);
		return voxel_at_local_unsafe(x, y, z);
	}

	inline Skwoxel::Voxel& Skwoxel::voxel_at_global(int x, int y, int z)
	{
		return voxel_at_local(x - lower_bounds.x, y - lower_bounds.y, z - lower_bounds.z);
	}

}

#endif // SKWOXEL_SKWOXEL_H
