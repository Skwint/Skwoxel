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

#include "lotsa.h"
#include "simplify.h"
#include "symetric_matrix.h"

using namespace godot;

namespace skwoxel
{
	Simplify::Simplify(lotsa<Vector3>& points, lotsa<int>& indices)
	{
		set(points, indices);
	}

	void Simplify::set(lotsa<Vector3>& points, lotsa<int>& indices)
	{
		int triangle_count = indices.size() / 3;
		triangles.resize(triangle_count);
		for (int i = 0; i < triangle_count; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				triangles[i].v[j] = indices[3 * i + j];
			}
		}
		int vertex_count = points.size();
		vertices.resize(vertex_count);
		for (int i = 0; i < vertex_count; ++i)
		{
			vertices[i].p = points[i];
		}
	}

	void Simplify::get(lotsa<Vector3>& points, lotsa<int>& indices)
	{
		int triangle_count = triangles.size();
		indices.resize(triangle_count * 3);
		for (int i = 0; i < triangle_count; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				indices[i * 3 + j] = triangles[i].v[j];
			}
		}
		int vertex_count = vertices.size();
		points.resize(vertex_count);
		for (int i = 0; i < vertex_count; ++i)
		{
			points[i] = vertices[i].p;
		}
	}

	//
	// Main simplification function 
	//
	// target_count  : target nr. of triangles
	// agressiveness : sharpness to increase the threshold.
	//                 5..8 are good numbers
	//                 more iterations yield higher quality
	//
	void Simplify::simplify_mesh(int target_count, double agressiveness)
	{
		// init
		for (int i = 0; i < triangles.size(); ++i)
			triangles[i].deleted = 0;

		// main iteration loop 

		int deleted_triangles = 0;
		lotsa<int> deleted0, deleted1;
		int triangle_count = triangles.size();

		for (int iteration = 0; iteration < 100; ++iteration)
		{
			// target number of triangles reached ? Then break
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

				for (int j = 0; j < 3; ++j)
				{
					if (t.err[j] < threshold)
					{
						int i0 = t.v[j];
						Vertex& v0 = vertices[i0];
						int i1 = t.v[(j + 1) % 3];
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
						v0.p = p;
						v0.q = v1.q + v0.q;
						int tstart = refs.size();

						update_triangles(i0, v0, deleted0, deleted_triangles);
						update_triangles(i0, v1, deleted1, deleted_triangles);

						int tcount = refs.size() - tstart;

						if (tcount <= v0.tcount)
						{
							// save ram
							if (tcount)
								memcpy(&refs[v0.tstart], &refs[tstart], tcount * sizeof(Ref));
						}
						else
							// append
							v0.tstart = tstart;

						v0.tcount = tcount;
						break;
					}
				}
				// done?
				if (triangle_count - deleted_triangles <= target_count)
					break;
			}
		}

		// clean up mesh
		compact_mesh();
	}

	// Check if a triangle flips when this edge is removed

	bool Simplify::flipped(Vector3 p, int i0, int i1, Vertex& v0, Vertex& v1, lotsa<int>& deleted)
	{
		int bordercount = 0;
		for (int k = 0; k < v0.tcount; ++k)
		{
			Triangle& t = triangles[refs[v0.tstart + k].tid];
			if (t.deleted)continue;

			int s = refs[v0.tstart + k].tvertex;
			int id1 = t.v[(s + 1) % 3];
			int id2 = t.v[(s + 2) % 3];

			if (id1 == i1 || id2 == i1) // delete ?
			{
				bordercount++;
				deleted[k] = 1;
				continue;
			}
			Vector3 d1 = vertices[id1].p - p; d1.normalize();
			Vector3 d2 = vertices[id2].p - p; d2.normalize();
			if (fabs(d1.dot(d2)) > 0.999) return true;
			Vector3 n;
			n = d1.cross(d2);
			n.normalize();
			deleted[k] = 0;
			if (n.dot(t.n) < 0.2) return true;
		}
		return false;
	}

	// Update triangle connections and edge error after a edge is collapsed

	void Simplify::update_triangles(int i0, Vertex& v, lotsa<int>& deleted, int& deleted_triangles)
	{
		Vector3 p;
		for (int k = 0; k < v.tcount; ++k)
		{
			Ref r = refs[v.tstart + k];
			Triangle& t = triangles[r.tid];
			if (t.deleted)continue;
			if (deleted[k])
			{
				t.deleted = 1;
				deleted_triangles++;
				continue;
			}
			t.v[r.tvertex] = i0;
			t.dirty = 1;
			t.err[0] = calculate_error(t.v[0], t.v[1], p);
			t.err[1] = calculate_error(t.v[1], t.v[2], p);
			t.err[2] = calculate_error(t.v[2], t.v[0], p);
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
				if (!triangles[i].deleted)
				{
					triangles[dst++] = triangles[i];
				}
			triangles.resize(dst);
		}
		//

		// Init Reference ID list
		for (int i = 0; i < vertices.size(); ++i)
		{
			vertices[i].tstart = 0;
			vertices[i].tcount = 0;
		}
		for (int i = 0; i < triangles.size(); ++i)
		{
			Triangle& t = triangles[i];
			for (int j = 0; j < 3; ++j) vertices[t.v[j]].tcount++;
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
			for (int j = 0; j < 3; ++j)
			{
				Vertex& v = vertices[t.v[j]];
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
					Triangle& t = triangles[k];
					for (int k = 0; k < 3; ++k)
					{
						int ofs = 0;
						int id = t.v[k];
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
							vcount[ofs]++;
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
				Vector3 n, p[3];
				for (int j = 0; j < 3; ++j) p[j] = vertices[t.v[j]].p;
				n = (p[1] - p[0]).cross(p[2] - p[0]);
				n.normalize();
				t.n = n;
				for (int j = 0; j < 3; ++j)
					vertices[t.v[j]].q = vertices[t.v[j]].q + SymetricMatrix(n.x, n.y, n.z, -n.dot(p[0]));
			}
			for (int i = 0; i < triangles.size(); ++i)
			{
				// Calc Edge Error
				Triangle& t = triangles[i]; Vector3 p;
				for (int j = 0; j < 3; ++j) t.err[j] = calculate_error(t.v[j], t.v[(j + 1) % 3], p);
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
			if (!triangles[i].deleted)
			{
				Triangle& t = triangles[i];
				triangles[dst++] = t;
				for (int j = 0; j < 3; ++j)vertices[t.v[j]].tcount = 1;
			}
		triangles.resize(dst);
		dst = 0;
		for (int i = 0; i < vertices.size(); ++i)
			if (vertices[i].tcount)
			{
				vertices[i].tstart = dst;
				vertices[dst].p = vertices[i].p;
				dst++;
			}
		for (int i = 0; i < triangles.size(); ++i)
		{
			Triangle& t = triangles[i];
			for (int j = 0; j < 3; ++j)t.v[j] = vertices[t.v[j]].tstart;
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
			Vector3 p1 = vertices[id_v1].p;
			Vector3 p2 = vertices[id_v2].p;
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
