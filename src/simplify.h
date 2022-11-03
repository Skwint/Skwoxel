/////////////////////////////////////////////
//
// Mesh Simplification Tutorial
//
// (C) by Sven Forstmann in 2014
//
// License : MIT
// http://opensource.org/licenses/MIT
//
//https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification
//
// 5/2016: Chris Rorden created minimal version for OSX/Linux/Windows compile
//
// 11/2022: Andy Newman made more minimal and reworked for Godot flavour data sets

#ifndef SKWOXEL_SIMPLIFY_H
#define SKWOXEL_SIMPLIFY_H

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#include "lotsa.h"
#include "symetric_matrix.h"

namespace skwoxel
{
	class Simplify
	{
	private:
		struct Triangle
		{
			double err[4];
			int deleted;
			int dirty;
			int attr;
			godot::Vector3 n;
		};
		struct Vertex
		{
			int tstart;
			int tcount;
			SymetricMatrix q;
			int border;
		};
		struct Ref
		{
			int tid;
			int tvertex;
		};

	public:
		Simplify();
		void simplify_mesh(lotsa<godot::Vector3>* p_points, lotsa<int>* p_indices, int target_count, double agressiveness = 7);

	private:
		double vertex_error(SymetricMatrix q, double x, double y, double z);
		double calculate_error(int id_v1, int id_v2, godot::Vector3& p_result);
		bool flipped(godot::Vector3 p, int i0, int i1, Vertex& v0, Vertex& v1, lotsa<int>& deleted);
		void update_triangles(int i0, Vertex& v, lotsa<int>& deleted, int& deleted_triangles);
		void update_mesh(int iteration);
		void compact_mesh();

	private:
		lotsa<godot::Vector3>* points;
		lotsa<int>* indices;
		lotsa<Triangle> triangles;
		lotsa<Vertex> vertices;
		lotsa<Ref> refs;
	};
}

#endif
