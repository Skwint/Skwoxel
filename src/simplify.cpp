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

#include "simplify.h"

#include <math.h>

#include <godot_cpp/variant/vector3.hpp>

using namespace godot;

namespace skwoxel
{
	//
	// Main simplification function
	//
	// target_count  : target nr. of triangles
	// agressiveness : sharpness to increase the threshold.
	//                 5..8 are good numbers
	//                 more iterations yield higher quality
	//
	void Simplify::simplify_mesh(lotsa<Vector3>* p_points, lotsa<int>* p_indices, int target_count, double agressiveness)
	{
		// init
		points = p_points;
		indices = p_indices;
		triangles.resize(indices->size() / 3);
		vertices.resize(points->size());
		for (int i = 0; i < triangles.size(); ++i)
		{
			triangles[i].deleted = 0;
		}

		// main iteration loop
		int deleted_triangles = 0;
		lotsa<int> deleted0, deleted1;
		int triangle_count = triangles.size();
		for (int iteration = 0; iteration < 100; iteration++)
		{
			if (triangle_count - deleted_triangles <= target_count)
				break;

			// update mesh once in a while
			if (iteration % 5 == 0)
			{
				update_mesh(iteration);
			}

			// clear dirty flag
			for (int i = 0; i < triangles.size(); ++i)
				triangles[i].dirty = 0;

			//
			// All triangles with edges below the threshold will be removed
			//
			// The following numbers works well for most models.
			// If it does not, try to adjust the 3 parameters
			//
			double threshold = 0.000000001 * pow(double(iteration + 3), agressiveness);

			// remove vertices & mark deleted triangles
			for (int i = 0; i < triangles.size(); ++i)
			{
				Triangle& t = triangles[i];
				if (t.err[3] > threshold) continue;
				if (t.deleted) continue;
				if (t.dirty) continue;

				int* index = &(*indices)[i * 3];
				for (int j = 0; j < 3; ++j)
				{
					if (t.err[j] < threshold)
					{
						int i0 = *(index + j);
						Vertex& v0 = vertices[i0];
						int i1 = *(index + (j + 1) % 3);
						Vertex& v1 = vertices[i1];
						// Border check
						if (v0.border != v1.border)  continue;

						// Compute vertex to collapse to
						Vector3 p;
						calculate_error(i0, i1, p);
						deleted0.resize(v0.tcount); // normals temporarily
						deleted1.resize(v1.tcount); // normals temporarily
						// don't remove if flipped
						if (flipped(p, i0, i1, v0, v1, deleted0)) continue;

						if (flipped(p, i1, i0, v1, v0, deleted1)) continue;

						// not flipped, so remove edge
						(*points)[i0] = p;
						v0.q = v1.q + v0.q;
						int tstart = refs.size();

						update_triangles(i0, v0, deleted0, deleted_triangles);
						update_triangles(i0, v1, deleted1, deleted_triangles);

						int tcount = refs.size() - tstart;

						if (tcount <= v0.tcount)
						{
							// save ram
							if (tcount)
							{
								memcpy(&refs[v0.tstart], &refs[tstart], tcount * sizeof(Ref));
							}
						}
						else
						{
							// append
							v0.tstart = tstart;
						}

						v0.tcount = tcount;
						break;
					}
				}
				// done?
				if (triangle_count - deleted_triangles <= target_count)break;
			}
		}
		// clean up mesh
		compact_mesh();
	} //simplify_mesh()

	// Check if a triangle flips when this edge is removed
	bool Simplify::flipped(Vector3 p, int i0, int i1, Vertex& v0, Vertex& v1, lotsa<int>& deleted)
	{
		for (int k = 0; k < v0.tcount; ++k)
		{
			int tidx = refs[v0.tstart + k].tid;
			Triangle& t = triangles[tidx];
			int* index = &(*indices)[tidx * 3];
			if (t.deleted)
				continue;

			int s = refs[v0.tstart + k].tvertex;
			int id1 = *(index + (s + 1) % 3);
			int id2 = *(index + (s + 2) % 3);

			if (id1 == i1 || id2 == i1) // delete ?
			{
				deleted[k] = 1;
				continue;
			}
			Vector3 d1 = (*points)[id1] - p;
			d1.normalize();
			Vector3 d2 = (*points)[id2] - p;
			d2.normalize();
			if (fabs(d1.dot(d2)) > 0.999)
				return true;
			Vector3 n = d1.cross(d2);
			n.normalize();
			deleted[k] = 0;
			if (n.dot(t.n) < 0.2)
				return true;
		}
		return false;
	}

	// Update triangle connections and edge error after a edge is collapsed
	void Simplify::update_triangles(int i0, Vertex& v, lotsa<int>& deleted, int& deleted_triangles)
	{
		Vector3 p;
		for (int k = 0; k < v.tcount; ++k)
		{
			Ref& r = refs[v.tstart + k];
			Triangle& t = triangles[r.tid];
			int* index = &(*indices)[r.tid * 3];
			if (t.deleted)
				continue;
			if (deleted[k])
			{
				t.deleted = 1;
				deleted_triangles++;
				continue;
			}
			index[r.tvertex] = i0;
			t.dirty = 1;
			t.err[0] = calculate_error(index[0], index[1], p);
			t.err[1] = calculate_error(index[1], index[2], p);
			t.err[2] = calculate_error(index[2], index[0], p);
			t.err[3] = MIN(t.err[0], MIN(t.err[1], t.err[2]));
			refs.push_back(r);
		}
	}

	// compact triangles, compute edge error and build reference list

	void Simplify::update_mesh(int iteration)
	{
		if (iteration > 0) // compact triangles
		{
			int dst = 0;
			for (int i = 0; i < triangles.size(); ++i)
			{
				int dst_idx = dst * 3;
				int src_idx = i * 3;
				if (!triangles[i].deleted)
				{
					triangles[dst++] = triangles[i];
					for (int j = 0; j < 3; ++j)
					{
						indices[dst_idx + j] = indices[src_idx + j];
					}
				}
			}
			triangles.resize(dst);
			indices->resize(dst * 3);
		}
		//

		// Init Reference ID list
		Vertex* vert = &vertices[0];
		for (int i = 0; i < vertices.size(); ++i)
		{
			vert->tstart = 0;
			vert->tcount = 0;
			++vert;
		}
		int* index = &(*indices)[0];
		for (int i = 0; i < triangles.size(); ++i)
		{
			for (int j = 0; j < 3; ++j)
				vertices[*index++].tcount++;
		}
		int tstart = 0;
		for (int i = 0; i < vertices.size(); ++i)
		{
			Vertex& v = vertices[i];
			v.tstart = tstart;
			tstart += v.tcount;
			v.tcount = 0;
		}

		// Write References
		refs.resize(triangles.size() * 3);
		for (int i = 0; i < triangles.size(); ++i)
		{
			Triangle& t = triangles[i];
			int* index = &(*indices)[i * 3];
			for (int j = 0; j < 3; ++j)
			{
				Vertex& v = vertices[index[j]];
				refs[v.tstart + v.tcount].tid = i;
				refs[v.tstart + v.tcount].tvertex = j;
				v.tcount++;
			}
		}

		// Init Quadrics by Plane & Edge Errors
		//
		// required at the beginning ( iteration == 0 )
		// recomputing during the simplification is not required,
		// but mostly improves the result for closed meshes
		//
		if (iteration == 0)
		{
			// Identify boundary : vertices[].border=0,1

			lotsa<int> vcount, vids;

			for (int i = 0; i < vertices.size(); ++i)
				vertices[i].border = 0;

			for (int i = 0; i < vertices.size(); ++i)
			{
				Vertex& v = vertices[i];
				vcount.clear();
				vids.clear();
				for (int j = 0; j < v.tcount; ++j)
				{
					int k = refs[v.tstart + j].tid;
					int* index = &(*indices)[k * 3];
					for (int k = 0; k < 3; ++k)
					{
						int ofs = 0, id = index[k];
						while (ofs < vcount.size())
						{
							if (vids[ofs] == id)break;
							ofs++;
						}
						if (ofs == vcount.size())
						{
							vcount.push_back(1);
							vids.push_back(id);
						}
						else
						{
							vcount[ofs]++;
						}
					}
				}
				for (int j = 0; j < vcount.size(); ++j)
				{
					if (vcount[j] == 1)
						vertices[vids[j]].border = 1;
				}
			}
			//initialize errors
			for (int i = 0; i < vertices.size(); ++i)
				vertices[i].q = SymetricMatrix(0.0);

			for (int i = 0; i < triangles.size(); ++i)
			{
				Triangle& t = triangles[i];
				int* index = &(*indices)[i * 3];
				Vector3 n;
				Vector3 p[3];
				for (int j = 0; j < 3; ++j)
					p[j] = (*points)[index[j]];
				n = (p[1] - p[0]).cross(p[2] - p[0]);
				n.normalize();
				t.n = n;
				for (int j = 0; j < 3; ++j)
					vertices[index[j]].q = vertices[index[j]].q + SymetricMatrix(n.x, n.y, n.z, -n.dot(p[0]));
			}
			for (int i = 0; i < triangles.size(); ++i)
			{
				// Calc Edge Error
				Triangle& t = triangles[i];
				int* index = &(*indices)[i * 3];
				Vector3 p;
				for (int j = 0; j < 3; ++j)
					t.err[j] = calculate_error(index[j], index[(j + 1) % 3], p);
				t.err[3] = MIN(t.err[0], MIN(t.err[1], t.err[2]));
			}
		}
	}

	// Finally compact mesh before exiting

	void Simplify::compact_mesh()
	{
		int dst = 0;
		for (int i = 0; i < vertices.size(); ++i)
		{
			vertices[i].tcount = 0;
		}
		for (int i = 0; i < triangles.size(); ++i)
		{
			if (!triangles[i].deleted)
			{
				Triangle& t = triangles[i];
				int* index = &(*indices)[i * 3];
				triangles[dst++] = t;
				for (int j = 0; j < 3; ++j)
					vertices[index[j]].tcount = 1;
			}
		}
		triangles.resize(dst);
		dst = 0;
		for (int i = 0; i < vertices.size(); ++i)
		{
			if (vertices[i].tcount)
			{
				vertices[i].tstart = dst;
				(*points)[dst] = (*points)[i];
				dst++;
			}
		}
		for (int i = 0; i < triangles.size(); ++i)
		{
			int* index = &(*indices)[i * 3];
			for (int j = 0; j < 3; ++j)
				index[j] = vertices[index[j]].tstart;
		}
		vertices.resize(dst);
	}

	// Error between vertex and Quadric

	double Simplify::vertex_error(SymetricMatrix q, double x, double y, double z)
	{
		return   q[0] * x * x + 2 * q[1] * x * y + 2 * q[2] * x * z + 2 * q[3] * x + q[4] * y * y
			+ 2 * q[5] * y * z + 2 * q[6] * y + q[7] * z * z + 2 * q[8] * z + q[9];
	}

	// Error for one edge

	double Simplify::calculate_error(int id_v1, int id_v2, Vector3& p_result)
	{
		// compute interpolated vertex

		SymetricMatrix q = vertices[id_v1].q + vertices[id_v2].q;
		bool   border = vertices[id_v1].border & vertices[id_v2].border;
		double error = 0;
		double det = q.det(0, 1, 2, 1, 4, 5, 2, 5, 7);
		if (det != 0 && !border)
		{

			// q_delta is invertible
			p_result.x = -1 / det * (q.det(1, 2, 3, 4, 5, 6, 5, 7, 8));	// vx = A41/det(q_delta)
			p_result.y = 1 / det * (q.det(0, 2, 3, 1, 5, 6, 2, 7, 8));	// vy = A42/det(q_delta)
			p_result.z = -1 / det * (q.det(0, 1, 3, 1, 4, 6, 2, 5, 8));	// vz = A43/det(q_delta)

			error = vertex_error(q, p_result.x, p_result.y, p_result.z);
		}
		else
		{
			// det = 0 -> try to find best result
			Vector3 p1 = (*points)[id_v1];
			Vector3 p2 = (*points)[id_v2];
			Vector3 p3 = (p1 + p2) / 2;
			double error1 = vertex_error(q, p1.x, p1.y, p1.z);
			double error2 = vertex_error(q, p2.x, p2.y, p2.z);
			double error3 = vertex_error(q, p3.x, p3.y, p3.z);
			error = MIN(error1, MIN(error2, error3));
			if (error1 == error) p_result = p1;
			if (error2 == error) p_result = p2;
			if (error3 == error) p_result = p3;
		}
		return error;
	}
}
