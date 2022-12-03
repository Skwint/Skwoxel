/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include "skwoxel.h"

#include <thread>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/concave_polygon_shape3d.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include "random.h"
#include "skwoxel_helpers.h"
#include "skwoxel_field.h"
#include "simplify.h"

#define SKWOXEL_MESH_NAME "SkwoxelMesh"
#define SKWOXEL_COLLISION_NAME "SkwoxelCollision"

using namespace godot;
using std::vector;
using std::thread;

namespace skwoxel
{
	void Skwoxel::_notification(int p_what) {
		// UtilityFunctions::print("Skwoxel note: ", String::num(p_what));
	}

	bool Skwoxel::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(lower_bounds);
		SKWOXEL_SET_METHOD(upper_bounds);
		SKWOXEL_SET_METHOD(remove_bubbles);
		SKWOXEL_SET_METHOD(remove_floaters);
		SKWOXEL_SET_METHOD(simple_normals);
		SKWOXEL_SET_METHOD(smooth_normals);
		SKWOXEL_SET_METHOD(minimum_edge);
		SKWOXEL_SET_METHOD(num_threads);
		SKWOXEL_SET_METHOD(randomize_seeds);
		SKWOXEL_SET_METHOD(generate);
		SKWOXEL_SET_METHOD(material);
		return false;
	}

	bool Skwoxel::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(lower_bounds);
		SKWOXEL_GET_METHOD(upper_bounds);
		SKWOXEL_GET_METHOD(remove_bubbles);
		SKWOXEL_GET_METHOD(remove_floaters);
		SKWOXEL_GET_METHOD(simple_normals);
		SKWOXEL_GET_METHOD(smooth_normals);
		SKWOXEL_GET_METHOD(minimum_edge);
		SKWOXEL_GET_METHOD(num_threads);
		SKWOXEL_GET_METHOD(randomize_seeds);
		SKWOXEL_GET_METHOD(generate);
		SKWOXEL_GET_METHOD(material);
		return false;
	}

	String Skwoxel::_to_string() const {
		return "[ GDExtension::Skwoxel <--> Instance ID:" + itos(get_instance_id()) + " ]";
	}

	void Skwoxel::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool Skwoxel::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool Skwoxel::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void Skwoxel::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, lower_bounds);
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, upper_bounds);
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, remove_bubbles);
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, remove_floaters);
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, simple_normals);
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, smooth_normals);
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, minimum_edge);
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, num_threads);
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, randomize_seeds);
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, generate);
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, material);
		ClassDB::bind_method(D_METHOD("generate"), &Skwoxel::generate);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material"), "set_material", "get_material");
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3I, lower_bounds);
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3I, upper_bounds);
		SKWOXEL_ADD_PROPERTY(Variant::BOOL, remove_bubbles);
		SKWOXEL_ADD_PROPERTY(Variant::BOOL, remove_floaters);
		SKWOXEL_ADD_PROPERTY(Variant::BOOL, simple_normals);
		SKWOXEL_ADD_PROPERTY(Variant::BOOL, smooth_normals);
		SKWOXEL_ADD_PROPERTY(Variant::FLOAT, minimum_edge);
		SKWOXEL_ADD_PROPERTY(Variant::INT, num_threads);
		SKWOXEL_ADD_PROPERTY(Variant::BOOL, randomize_seeds);
		SKWOXEL_ADD_PROPERTY(Variant::BOOL, generate);
	}

	Skwoxel::Skwoxel() :
		remove_bubbles(true),
		remove_floaters(true),
		simple_normals(true),
		smooth_normals(true),
		minimum_edge(0.02)
	{
	}

	Skwoxel::~Skwoxel()
	{
	}

	void Skwoxel::report(const String& str)
	{
		/**/
		auto end_time = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds_start = end_time - start_time;
		std::chrono::duration<double> elapsed_seconds_last = end_time - last_time;
		last_time = end_time;
		UtilityFunctions::print("<Skwoxel> ", String::num(elapsed_seconds_start.count()), " : ", str, " (", String::num(elapsed_seconds_last.count()), ")");
		/**/
	}

	void Skwoxel::generate()
	{
		start_time = std::chrono::steady_clock::now();
		last_time = start_time;

		delete_mesh();
		air_points.clear();
		ground_points.clear();
		collect_children();
		report("Children collected");
		root.pre_generate(randomize_seeds, num_threads);
		report("Voxel pre generate complete");
		generate_voxels();
		report("Voxel generation complete");
		root.post_generate(air_points, ground_points);
		report("Voxel post generate complete");
		filter();
		report("Filtering complete");
		generate_mesh();
		report("Mesh generation complete");
		delete_voxels();

	}

	void Skwoxel::delete_mesh()
	{
		auto mesh = find_child(SKWOXEL_MESH_NAME, false, false);
		if (mesh)
		{
			remove_child(mesh);
			mesh->queue_free();
		}
		auto coll = find_child(SKWOXEL_COLLISION_NAME, false, false);
		if (coll)
		{
			remove_child(coll);
			coll->queue_free();
		}
	}

	void Skwoxel::allocate_voxels()
	{
		voxels.resize(data_size_x() * data_size_y() * data_size_z());
	}

	void Skwoxel::delete_voxels()
	{
		voxels.clear();
	}

	void Skwoxel::generate_air_flags()
	{
		bool changed = true;
		for (auto& air : air_points)
		{
			voxel_at_global(round(air.x), round(air.y), round(air.z)).flags |= Voxel::AIR;
		}
		while (changed)
		{
			changed = false;
			for (int z = 0; z < size_z(); ++z)
			{
				for (int y = 0; y < size_y(); ++y)
				{
					// Sweep right
					bool is_air = false;
					Voxel* voxel = &voxel_at_local_unsafe(0, y, z);
					for (int x = 0; x < size_x(); ++x)
					{
						if (is_air)
						{
							if (voxel->strength <= 0.0)
							{
								if (!(voxel->flags & Voxel::AIR))
								{
									changed = true;
									voxel->flags |= Voxel::AIR;
								}
							}
							else
							{
								is_air = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::AIR)
							{
								is_air = true;
							}
						}
						++voxel;
					}
					// Sweep left
					is_air = false;
					voxel = &voxel_at_local_unsafe(size_x() - 1, y, z);
					for (int x = size_x(); x > 0; --x)
					{
						if (is_air)
						{
							if (voxel->strength <= 0.0)
							{
								if (!(voxel->flags & Voxel::AIR))
								{
									changed = true;
									voxel->flags |= Voxel::AIR;
								}
							}
							else
							{
								is_air = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::AIR)
							{
								is_air = true;
							}
						}
						--voxel;
					}
				}
			}
			for (int z = 0; z < size_z(); ++z)
			{
				for (int x = 0; x < size_x(); ++x)
				{
					// Sweep up
					bool is_air = false;
					Voxel* voxel = &voxel_at_local_unsafe(x, 0, z);
					for (int y = 0; y < size_y(); ++y)
					{
						if (is_air)
						{
							if (voxel->strength <= 0.0)
							{
								if (!(voxel->flags & Voxel::AIR))
								{
									changed = true;
									voxel->flags |= Voxel::AIR;
								}
							}
							else
							{
								is_air = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::AIR)
							{
								is_air = true;
							}
						}
						voxel += data_size_x();
					}
					// Sweep down
					is_air = false;
					voxel = &voxel_at_local_unsafe(x, size_y() - 1, z);
					for (int y = size_y(); y > 0; --y)
					{
						if (is_air)
						{
							if (voxel->strength <= 0.0)
							{
								if (!(voxel->flags & Voxel::AIR))
								{
									changed = true;
									voxel->flags |= Voxel::AIR;
								}
							}
							else
							{
								is_air = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::AIR)
							{
								is_air = true;
							}
						}
						voxel -= data_size_x();
					}
				}
			}
			for (int y = 0; y < size_y(); ++y)
			{
				for (int x = 0; x < size_x(); ++x)
				{
					// Sweep away
					bool is_air = false;
					Voxel* voxel = &voxel_at_local_unsafe(x, y, 0);
					for (int z = 0; z < size_z(); ++z)
					{
						if (is_air)
						{
							if (voxel->strength <= 0.0)
							{
								if (!(voxel->flags & Voxel::AIR))
								{
									changed = true;
									voxel->flags |= Voxel::AIR;
								}
							}
							else
							{
								is_air = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::AIR)
							{
								is_air = true;
							}
						}
						voxel += data_size_x() * data_size_y();
					}
					// Sweep down
					is_air = false;
					voxel = &voxel_at_local_unsafe(x, y, size_z() - 1);
					for (int z = size_z(); z > 0; --z)
					{
						if (is_air)
						{
							if (voxel->strength <= 0.0)
							{
								if (!(voxel->flags & Voxel::AIR))
								{
									changed = true;
									voxel->flags |= Voxel::AIR;
								}
							}
							else
							{
								is_air = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::AIR)
							{
								is_air = true;
							}
						}
						voxel -= data_size_x() * data_size_y();
					}
				}
			}
		}
	}

	void Skwoxel::generate_ground_flags()
	{
		bool changed = true;
		for (auto& ground : ground_points)
		{
			voxel_at_global(round(ground.x), round(ground.y), round(ground.z)).flags |= Voxel::GROUND;
		}
		while (changed)
		{
			changed = false;
			for (int z = 0; z < size_z(); ++z)
			{
				for (int y = 0; y < size_y(); ++y)
				{
					// Sweep right
					bool is_ground = false;
					Voxel* voxel = &voxel_at_local_unsafe(0, y, z);
					for (int x = 0; x < size_x(); ++x)
					{
						if (is_ground)
						{
							if (voxel->strength > 0.0)
							{
								if (!(voxel->flags & Voxel::GROUND))
								{
									voxel->flags |= Voxel::GROUND;
									changed = true;
								}
							}
							else
							{
								is_ground = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::GROUND)
							{
								is_ground = true;
							}
						}
						++voxel;
					}
					// Sweep left
					is_ground = false;
					voxel = &voxel_at_local_unsafe(size_x() - 1, y, z);
					for (int x = size_x(); x > 0; --x)
					{
						if (is_ground)
						{
							if (voxel->strength > 0.0)
							{
								if (!(voxel->flags & Voxel::GROUND))
								{
									voxel->flags |= Voxel::GROUND;
									changed = true;
								}
							}
							else
							{
								is_ground = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::GROUND)
							{
								is_ground = true;
							}
						}
						--voxel;
					}
				}
			}
			for (int z = 0; z < size_z(); ++z)
			{
				for (int x = 0; x < size_x(); ++x)
				{
					// Sweep up
					bool is_ground = false;
					Voxel* voxel = &voxel_at_local_unsafe(x, 0, z);
					for (int y = 0; y < size_y(); ++y)
					{
						if (is_ground)
						{
							if (voxel->strength > 0.0)
							{
								if (!(voxel->flags & Voxel::GROUND))
								{
									voxel->flags |= Voxel::GROUND;
									changed = true;
								}
							}
							else
							{
								is_ground = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::GROUND)
							{
								is_ground = true;
							}
						}
						voxel += data_size_x();
					}
					// Sweep down
					is_ground = false;
					voxel = &voxel_at_local_unsafe(x, size_y() - 1, z);
					for (int y = size_y(); y > 0; --y)
					{
						if (is_ground)
						{
							if (voxel->strength > 0.0)
							{
								if (!(voxel->flags & Voxel::GROUND))
								{
									voxel->flags |= Voxel::GROUND;
									changed = true;
								}
							}
							else
							{
								is_ground = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::GROUND)
							{
								is_ground = true;
							}
						}
						voxel -= data_size_x();
					}
				}
			}
			for (int y = 0; y < size_y(); ++y)
			{
				for (int x = 0; x < size_x(); ++x)
				{
					// Sweep away
					bool is_ground = false;
					Voxel* voxel = &voxel_at_local_unsafe(x, y, 0);
					for (int z = 0; z < size_z(); ++z)
					{
						if (is_ground)
						{
							if (voxel->strength > 0.0)
							{
								if (!(voxel->flags & Voxel::GROUND))
								{
									voxel->flags |= Voxel::GROUND;
									changed = true;
								}
							}
							else
							{
								is_ground = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::GROUND)
							{
								is_ground = true;
							}
						}
						voxel += data_size_x() * data_size_y();
					}
					// Sweep towards
					is_ground = false;
					voxel = &voxel_at_local_unsafe(x, y, size_z() - 1);
					for (int z = size_z(); z > 0; --z)
					{
						if (is_ground)
						{
							if (voxel->strength > 0.0)
							{
								if (!(voxel->flags & Voxel::GROUND))
								{
									voxel->flags |= Voxel::GROUND;
									changed = true;
								}
							}
							else
							{
								is_ground = false;
							}
						}
						else
						{
							if (voxel->flags & Voxel::GROUND)
							{
								is_ground = true;
							}
						}
						voxel -= data_size_x() * data_size_y();
					}
				}
			}
		}
	}
	
	void Skwoxel::filter()
	{
		if (remove_bubbles)
		{
			filter_bubbles();
		}
		if (remove_floaters)
		{
			filter_floaters();
		}
	}

	void Skwoxel::filter_bubbles()
	{
		if (air_points.size() == 0)
		{
			UtilityFunctions::print("Cannot remove bubbles because no air triggers exist to say what isn't a bubble.");
			return;
		}
		generate_air_flags();
		for (int z = 0; z < size_z(); ++z)
		{
			for (int y = 0; y < size_y(); ++y)
			{
				Voxel* voxel = &voxel_at_local_unsafe(0, y, z);
				for (int x = 0; x < size_x(); ++x)
				{
					if (voxel->strength <= 0.0 && !(voxel->flags & Voxel::AIR))
					{
						voxel->strength = 1.0;
					}
					++voxel;
				}
			}
		}
	}

	void Skwoxel::filter_floaters()
	{
		if (ground_points.size() == 0)
		{
			UtilityFunctions::print("Cannot remove floaters because no ground triggers exist to say what isn't a floater.");
			return;
		}
		generate_ground_flags();
		for (int z = 0; z < size_z(); ++z)
		{
			for (int y = 0; y < size_y(); ++y)
			{
				Voxel* voxel = &voxel_at_local_unsafe(0, y, z);
				for (int x = 0; x < size_x(); ++x)
				{
					if (voxel->strength > 0.0 && !(voxel->flags & Voxel::GROUND))
					{
						voxel->strength = -0.001;
					}
					++voxel;
				}
			}
		}
	}

	real_t Skwoxel::sample(const Vector3& pos, int thread_num) const
	{
		return root.strength(pos, pos, thread_num);
	}

	void Skwoxel::collect_children()
	{
		root.collect_children_of(this);
	}

	void Skwoxel::generate_voxels()
	{
		delete_voxels();
		allocate_voxels();

		if (num_threads < 2)
		{
			generate_voxels_thread(lower_bounds.z, upper_bounds.z, 0);
		}
		else
		{
			int zsize = upper_bounds.z + 1 - lower_bounds.z;
			int dz = zsize / num_threads;
			if (dz * num_threads < zsize)
				dz++;

			vector<thread> threads;
			int thread_num = 0;
			for (int low = lower_bounds.z; low < upper_bounds.z; low += dz)
			{
				int high = low + dz - 1;
				if (high > upper_bounds.z)
				{
					high = upper_bounds.z;
				}
				threads.push_back(thread(&Skwoxel::generate_voxels_thread, this, low, high, thread_num));
			}
			for (auto& tt : threads)
			{
				tt.join();
			}
			threads.clear();
		}
	}

	void Skwoxel::generate_voxels_thread(int lowz, int highz, int thread_num)
	{
		for (int z = lowz; z <= highz; ++z)
		{
			for (int y = lower_bounds.y; y <= upper_bounds.y; ++y)
			{
				Voxel* voxel = &voxel_at_global_unsafe(lower_bounds.x, y, z);
				for (int x = lower_bounds.x; x <= upper_bounds.x; ++x)
				{
					voxel->strength = sample(Vector3(x, y, z), thread_num);
					voxel->flags = 0;
					++voxel;
				}
			}
		}
	}

	void Skwoxel::generate_mesh()
	{
		// These are the index steps corresponding to each of the 7 edges
		const Vector3 steps[] =
		{
			Vector3(1.0, 0.0, 0.0),
			Vector3(0.0, 1.0, 0.0),
			Vector3(0.0, 0.0, 1.0),
			Vector3(1.0, 1.0, 0.0),
			Vector3(1.0, 0.0, 1.0),
			Vector3(0.0, 1.0, 1.0),
			Vector3(1.0, 1.0, 1.0),
		};
		// These are the index offsets for the voxels on the corners of a cube
		const int32_t cube_idx[] =
		{
			0,
			1,
			data_size_x(),
			data_size_x() + 1,
			data_size_y() * data_size_x(),
			data_size_y() * data_size_x() + 1,
			data_size_y() * data_size_x() + data_size_x(),
			data_size_y() * data_size_x() + data_size_x() + 1
		};
		// These are the index offsets for the voxels on each end of a tetrahedron edge
		// and the index into the edge in the owning voxels array
		struct TetraEdge
		{
			int32_t idx[2];
			Voxel::EdgeNum edge;
		};
		TetraEdge tetraEdges[] =
		{
			{ {cube_idx[0], cube_idx[1]}, Voxel::EEN_X },
			{ {cube_idx[0], cube_idx[2]}, Voxel::EEN_Y },
			{ {cube_idx[0], cube_idx[4]}, Voxel::EEN_Z },
			{ {cube_idx[0], cube_idx[3]}, Voxel::EEN_XY },
			{ {cube_idx[0], cube_idx[5]}, Voxel::EEN_XZ },
			{ {cube_idx[0], cube_idx[6]}, Voxel::EEN_YZ },
			{ {cube_idx[0], cube_idx[7]}, Voxel::EEN_XYZ },

			{ {cube_idx[6], cube_idx[7]}, Voxel::EEN_X }, //7
			{ {cube_idx[5], cube_idx[7]}, Voxel::EEN_Y },
			{ {cube_idx[3], cube_idx[7]}, Voxel::EEN_Z },
			{ {cube_idx[4], cube_idx[7]}, Voxel::EEN_XY },
			{ {cube_idx[2], cube_idx[7]}, Voxel::EEN_XZ },
			{ {cube_idx[1], cube_idx[7]}, Voxel::EEN_YZ },

			{ {cube_idx[2], cube_idx[3]}, Voxel::EEN_X }, //13
			{ {cube_idx[4], cube_idx[5]}, Voxel::EEN_X },
			{ {cube_idx[1], cube_idx[3]}, Voxel::EEN_Y },
			{ {cube_idx[4], cube_idx[6]}, Voxel::EEN_Y },
			{ {cube_idx[1], cube_idx[5]}, Voxel::EEN_Z },
			{ {cube_idx[2], cube_idx[6]}, Voxel::EEN_Z }
		};
		// These are the collections of 4 corners and 6 edges which form each tetrahedron
		struct Tetra
		{
			int32_t voxIdx[4];
			int32_t edgeIdx[6];
		};
		Tetra tetras[] =
		{
			{ {0, 1, 3, 7},    {0, 3, 6, 15, 12,  9} },
			{ {0, 3, 2, 7},    {3, 1, 6, 13,  9, 11} },
			{ {0, 2, 6, 7},    {1, 5, 6, 18, 11,  7} },
			{ {0, 6, 4, 7},    {5, 2, 6, 16,  7, 10} },
			{ {0, 4, 5, 7},    {2, 4, 6, 14, 10,  8} },
			{ {0, 5, 1, 7},    {4, 0, 6, 17,  8, 12} }
		};
	
		// First, count the vertices and link the edges
		int32_t numVertices = 0;
		for (int32_t z = 0; z < size_z(); ++z)
		{
			for (int32_t y = 0; y < size_y(); ++y)
			{
				Voxel* vox = &voxel_at_local_unsafe(0, y, z);
				for (int32_t x = 0; x < size_x(); ++x)
				{
					// find indices for all edges owned by this voxel:
					TetraEdge* edge = tetraEdges;
					bool inside = vox->strength > 0.0f;
					for (int32_t e = 0; e < Voxel::EEN_COUNT; ++e)
					{
						if (((vox + edge->idx[1])->strength > 0.0f) != inside)
						{
							vox->edges[edge->edge] = numVertices;
							++numVertices;
						}
						else
						{
							vox->edges[edge->edge] = -1;
						}
						++edge;
					}
					++vox;
				}
			}
		}

		// Now that we know how many vertices there will be we can allocate a buffer for them
		lotsa<Vector3> vertices;
		vertices.resize(numVertices);

		Vector3 start;
		start.z = lower_bounds.z;
		for (int32_t z = 0; z < size_z(); ++z)
		{
			start.y = lower_bounds.y;
			for (int32_t y = 0; y < size_y(); ++y)
			{
				start.x = lower_bounds.x;
				Voxel* vox = &voxel_at_local(0, y, z);
				for (int32_t x = 0; x < size_x(); ++x)
				{
					TetraEdge* edge = tetraEdges;
					for (uint32_t e = 0; e < Voxel::EEN_COUNT; ++e)
					{
						if (vox->edges[e] != -1)
						{
							vertices[vox->edges[e]] = start + steps[e] * ((0.0f - vox->strength) / ((vox + edge->idx[1])->strength - vox->strength));
						}
						++edge;
					}
					++vox;
					start.x++;
				}
				start.y++;
			}
			start.z++;
		}
		report("- Vertex generation complete");

		// Create all the triangles.
		lotsa<int> indices;
		for (int32_t z = 0; z < size_z() - 1; ++z)
		{
			for (int32_t y = 0; y < size_y() - 1; ++y)
			{
				Voxel* vox = &voxel_at_local(0, y, z);
				for (int32_t x = 0; x < size_x() - 1; ++x)
				{
					bool inside[8];
					for (int32_t i = 0; i < 8; ++i)
					{
						inside[i] = ((vox + cube_idx[i])->strength > 0.0f);
					}
					Tetra* tet = tetras;
					for (int32_t t = 0; t < 6; ++t)
					{
						uint32_t mask = 0;
						for (int32_t v = 0; v < 4; ++v)
						{
							if (inside[tet->voxIdx[v]])
							{
								mask |= 1 << v;
							}
						}
#						define MT_INDEX(ee) \
						{ \
							TetraEdge * tedge = &tetraEdges[tet->edgeIdx[ee]]; \
							indices.push_back((vox + tedge->idx[0])->edges[tedge->edge]); \
						}
#						define MT_TRIANGLE(e1, e2, e3) { MT_INDEX(e3); MT_INDEX(e2); MT_INDEX(e1); }
						switch (mask)
						{
						case 0:
						case 15:
							break;
						case 1:
							MT_TRIANGLE(1, 2, 0);
							break;
						case 14:
							MT_TRIANGLE(2, 1, 0);
							break;
						case 2:
							MT_TRIANGLE(0, 4, 3);
							break;
						case 13:
							MT_TRIANGLE(0, 3, 4);
							break;
						case 4:
							MT_TRIANGLE(1, 3, 5);
							break;
						case 11:
							MT_TRIANGLE(1, 5, 3);
							break;
						case 8:
							MT_TRIANGLE(2, 5, 4);
							break;
						case 7:
							MT_TRIANGLE(2, 4, 5);
							break;
						case 3:
							MT_TRIANGLE(1, 2, 4);
							MT_TRIANGLE(1, 4, 3);
							break;
						case 12:
							MT_TRIANGLE(1, 4, 2);
							MT_TRIANGLE(1, 3, 4);
							break;
						case 5:
							MT_TRIANGLE(0, 3, 5);
							MT_TRIANGLE(0, 5, 2);
							break;
						case 10:
							MT_TRIANGLE(0, 5, 3);
							MT_TRIANGLE(0, 2, 5);
							break;
						case 9:
							MT_TRIANGLE(1, 5, 4);
							MT_TRIANGLE(0, 1, 4);
							break;
						case 6:
							MT_TRIANGLE(1, 4, 5);
							MT_TRIANGLE(0, 4, 1);
							break;
						}
#				undef MT_INDEX
#				undef MT_TRIANGLE
						++tet;
					}
					++vox;
				}
			}
		}
		report("- Triangle generation complete");

		// Simplify:
		// simplifying the mesh too much can move vertices so far out of position that the smooth normals
		// go very wrong and simple normals should be used instead.
		if (minimum_edge > 0.0)
		{
			Simplify simple(vertices, indices);
			simple.simplify_mesh(minimum_edge);
			simple.get(vertices, indices);
			report("- Simplification complete");
		}


		// Generate normals:
		// Note that if we have simple AND smooth set it will calculate both
		// and then use simple if the smooth results look weird.
		lotsa<Vector3> normals;
		normals.resize(vertices.size());
		if (simple_normals)
		{
			for (int idx = 0; idx < indices.size(); idx += 3)
			{
				int idx0 = indices[idx];
				int idx1 = indices[idx + 1];
				int idx2 = indices[idx + 2];
				Vector3 norm = (vertices[idx2] - vertices[idx0]).cross(vertices[idx1] - vertices[idx0]);//.normalized();
				normals[idx0] += norm;
				normals[idx1] += norm;
				normals[idx2] += norm;
			}
		}
		if (smooth_normals)
		{
			// Each smooth normal costs 4 calls to the field sampling function,
			// but because there are so few of them compared to the original voxel
			// count the impact on total time taken is small.
			Vector3 neighbour;
			Vector3 pos;
			for (int idx = 0; idx < vertices.size(); ++idx)
			{
				Vector3 gradient;
				pos = vertices[idx];
				real_t localStrength = sample(pos);
				neighbour = pos;
				neighbour.x += normal_offset;
				gradient.x = sample(neighbour) - localStrength;
				neighbour.x = pos.x;
				neighbour.y += normal_offset;
				gradient.y = sample(neighbour) - localStrength;
				neighbour.y = pos.y;
				neighbour.z += normal_offset;
				gradient.z = sample(neighbour) - localStrength;
				if ((!simple_normals) || gradient.dot(normals[idx]) < 0.0)
				{
					gradient.normalize();
					normals[idx] = -gradient;
				}
				else
				{
					normals[idx].normalize();
				}
			}
		}
		else
		{
			for (int idx = 0; idx < vertices.size(); ++idx)
			{
				normals[idx].normalize();
			}
		}

		report("- Normals generated");

		// Build arrays and mesh
		Array arrays;
		PackedVector3Array packed_vertices;
		PackedVector3Array packed_normals;
		PackedInt32Array packed_indices;

		packed_vertices.resize(vertices.size());
		memcpy(packed_vertices.ptrw(), &vertices[0], vertices.size() * sizeof(Vector3));
		packed_normals.resize(normals.size());
		memcpy(packed_normals.ptrw(), &normals[0], normals.size() * sizeof(Vector3));
		packed_indices.resize(indices.size());
		memcpy(packed_indices.ptrw(), &indices[0], indices.size() * sizeof(int));

		UtilityFunctions::print("Mesh generated with ", String::num(packed_indices.size() / 3), " triangles");

		arrays.resize(Mesh::ARRAY_MAX);
		arrays[Mesh::ARRAY_VERTEX] = packed_vertices;
		arrays[Mesh::ARRAY_NORMAL] = packed_normals;
		arrays[Mesh::ARRAY_INDEX] = packed_indices;
		Ref<ArrayMesh> mesh;
		mesh.instantiate();
		mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
		MeshInstance3D* mesh_instance = memnew(MeshInstance3D);
		mesh_instance->set_mesh(mesh);
		mesh_instance->set_name(SKWOXEL_MESH_NAME);
		if (material.is_valid())
		{
			mesh_instance->set_surface_override_material(0, material);
		}
		add_child(mesh_instance);
		report("- Graphics mesh generated");

		// Generate a corresponding collision shape
		PackedVector3Array physics;
		physics.resize(indices.size());
		for (int i = 0; i < indices.size(); ++i)
		{
			physics[i] = vertices[indices[i]];
		}
		Ref<ConcavePolygonShape3D> shape;
		shape.instantiate();
		shape->set_faces(physics);
		CollisionShape3D* collision = memnew(CollisionShape3D);
		collision->set_shape(shape);
		collision->set_name(SKWOXEL_COLLISION_NAME);
		add_child(collision);
		report("- Collision mesh generated");
	}

	// Properties.
	void Skwoxel::set_lower_bounds(const Vector3i& bounds) {
		lower_bounds = bounds;
	}

	Vector3i Skwoxel::get_lower_bounds() const {
		return lower_bounds;
	}

	void Skwoxel::set_upper_bounds(const Vector3i& bounds) {
		upper_bounds = bounds;
	}

	Vector3i Skwoxel::get_upper_bounds() const {
		return upper_bounds;
	}

	void Skwoxel::set_remove_bubbles(bool remove) {
		remove_bubbles = remove;
	}

	bool Skwoxel::get_remove_bubbles() const {
		return remove_bubbles;
	}

	void Skwoxel::set_remove_floaters(bool remove) {
		remove_floaters = remove;
	}

	bool Skwoxel::get_remove_floaters() const {
		return remove_floaters;
	}
}
