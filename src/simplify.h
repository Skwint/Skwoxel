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

#include "symetric_matrix.h"
#include "lotsa.h"

namespace skwoxel
{
	class Simplify
	{
	private:
		struct Triangle
		{
			int v[3];
			double err[4];
			int deleted;
			int dirty;
			godot::Vector3 n;
		};
		struct Vertex
		{
			godot::Vector3 p;
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
		Simplify(lotsa<godot::Vector3>& points, lotsa<int>& indices);
		void set(lotsa<godot::Vector3>& points, lotsa<int>& indices);
		void get(lotsa<godot::Vector3>& points, lotsa<int>& indices);
		void simplify_mesh(int target_count, double agressiveness = 7);

	private:
		double vertex_error(SymetricMatrix q, double x, double y, double z);
		double calculate_error(int id_v1, int id_v2, godot::Vector3& p_result);
		bool flipped(godot::Vector3 p, int i0, int i1, Vertex& v0, Vertex& v1, lotsa<int>& deleted);
		void update_triangles(int i0, Vertex& v, lotsa<int>& deleted, int& deleted_triangles);
		void update_mesh(int iteration);
		void compact_mesh();

	private:
		lotsa<Triangle> triangles;
		lotsa<Vertex> vertices;
		lotsa<Ref> refs;
	};
}
