/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#include "skwoxel.h"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include "skwoxel_helpers.h"
#include "skwoxel_field.h"

using namespace godot;

namespace skwoxel
{
	void Skwoxel::_notification(int p_what) {
		UtilityFunctions::print("Skwoxel note: ", String::num(p_what));
	}

	bool Skwoxel::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(lower_bounds);
		SKWOXEL_SET_METHOD(upper_bounds);
		SKWOXEL_SET_METHOD(generate);
		return false;
	}

	bool Skwoxel::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(lower_bounds);
		SKWOXEL_GET_METHOD(upper_bounds);
		SKWOXEL_GET_METHOD(generate);
		return false;
	}

	String Skwoxel::_to_string() const {
		return "[ GDExtension::Skwoxel <--> Instance ID:" + itos(get_instance_id()) + " ]";
	}

	void Skwoxel::_get_property_list(List<PropertyInfo>* list) const {
		list->push_back(PropertyInfo(Variant::VECTOR3I, "lower_bounds"));
		list->push_back(PropertyInfo(Variant::VECTOR3I, "upper_bounds"));
		list->push_back(PropertyInfo(Variant::BOOL, "generate"));
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
		SKWOXEL_BIND_SET_GET_METHOD(Skwoxel, generate);
		ClassDB::bind_method(D_METHOD("generate"), &Skwoxel::generate);
		ClassDB::bind_method(D_METHOD("generate_fields"), &Skwoxel::generate_fields);
		ClassDB::bind_method(D_METHOD("generate_mesh"), &Skwoxel::generate_mesh);
	}

	Skwoxel::Skwoxel() :
		voxels(0),
		fields(0)
	{
	}

	Skwoxel::~Skwoxel()
	{
		delete_voxels();
	}

	void Skwoxel::generate()
	{
		generate_fields();
		generate_mesh();
	}

	void Skwoxel::allocate_voxels()
	{
		if (!voxels)
		{
			voxels = new Voxel[data_size_x() * data_size_y() * data_size_z()];
		}
	}

	void Skwoxel::delete_voxels()
	{
		delete[] voxels;
		voxels = 0;
	}

	void Skwoxel::clear_voxels()
	{
		Voxel * voxel = voxels;
		for (int z = lower_bounds.z; z <= upper_bounds.z; ++z)
		{
			for (int y = lower_bounds.y; y <= upper_bounds.y; ++y)
			{
				for (int x = lower_bounds.x; x <= upper_bounds.x; ++x)
				{
					voxel->strength = 0.0;
					++voxel;
				}
			}
		}
	}

	real_t Skwoxel::sample(const Vector3& pos) const
	{
		real_t strength = 0.0;
		for (int idx = 0; idx < num_fields; ++idx)
		{
			strength += ((SkwoxelField *)(fields[idx]))->strength(pos);
		}
		return strength;
	}

	void Skwoxel::collect_children()
	{
		UtilityFunctions::print(__FUNCTION__);
		if (fields)
		{
			delete[] fields;
			fields = 0;
		}
		int count = get_child_count();
		int field_count = 0;
		for (int i = 0; i < count; i++)
		{
			Node* node = get_child(i);
			SkwoxelField* child = dynamic_cast<SkwoxelField*>(node);
			if (child)
			{
				++field_count;
			}
		}

		// Why am I using my own array instead of a TypedArray?
		// Because TypedArray is really annoying! Yay!
		// Is this safe? NO! The children might cease to exist
		// and we will still have pointers to them.
		// This is very very internal. Do not expose to scripts.
		num_fields = field_count;
		fields = new SkwoxelField * [field_count];

		field_count = 0;
		for (int i = 0; i < count; i++)
		{
			Node* node = get_child(i);
			SkwoxelField* child = dynamic_cast<SkwoxelField*>(node);
			if (child)
			{
				fields[field_count] = child;
				++field_count;
			}
		}
	}

	void Skwoxel::generate_fields()
	{
		UtilityFunctions::print(__FUNCTION__);
		allocate_voxels();
		collect_children();

		for (int z = lower_bounds.z; z <= upper_bounds.z; ++z)
		{
			for (int y = lower_bounds.y; y <= upper_bounds.y; ++y)
			{
				Voxel* voxel = &voxel_at_global_unsafe(lower_bounds.x, y, z);
				for (int x = lower_bounds.x; x <= upper_bounds.x; ++x)
				{
					voxel->strength = sample(Vector3(x, y, z));
					++voxel;
				}
			}
		}
	}

	void Skwoxel::generate_mesh()
	{
		UtilityFunctions::print(__FUNCTION__);
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

		// Create the array of arrays we need
		Array arrays;
		arrays.resize(Mesh::ARRAY_MAX);
		PackedVector3Array vertices;
		PackedVector3Array normals;
		PackedInt32Array indices;

		// Now that we know how many vertices there will be we can allocate a buffer for them
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

		// Create all the triangles.
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
//#						define MT_TRIANGLE(e1, e2, e3) { MT_INDEX(e1); MT_INDEX(e2); MT_INDEX(e3); }
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

		// Generate normals:
		real_t off = 0.2;
		normals.resize(numVertices);
		Vector3 neighbour;
		Vector3 pos;
		for (int idx = 0; idx < numVertices; ++idx)
		{
			Vector3 gradient;
			pos = vertices[idx];
			real_t localStrength = sample(pos);
			neighbour = pos;
			neighbour.x += off;
			gradient.x = sample(neighbour) - localStrength;
			neighbour.x = pos.x;
			neighbour.y += off;
			gradient.y = sample(neighbour) - localStrength;
			neighbour.y = pos.y;
			neighbour.z += off;
			gradient.z = sample(neighbour) - localStrength;
			gradient.normalize();
			normals[idx] = -gradient;
		}

		arrays[Mesh::ARRAY_VERTEX] = vertices;
		arrays[Mesh::ARRAY_NORMAL] = normals;
		arrays[Mesh::ARRAY_INDEX] = indices;
		Ref<ArrayMesh> mesh;
		mesh.instantiate();
		mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
		MeshInstance3D* mesh_instance = memnew(MeshInstance3D);
		mesh_instance->set_mesh(mesh);
		get_parent()->add_child(mesh_instance); // TODO - SAFETY CHECK and warnings
	}

	// Properties.
	void Skwoxel::set_lower_bounds(const Vector3i& bounds) {
		lower_bounds = bounds;
		delete_voxels();
	}

	Vector3i Skwoxel::get_lower_bounds() const {
		return lower_bounds;
	}

	void Skwoxel::set_upper_bounds(const Vector3i& bounds) {
		upper_bounds = bounds;
		delete_voxels();
	}

	Vector3i Skwoxel::get_upper_bounds() const {
		return upper_bounds;
	}
}
