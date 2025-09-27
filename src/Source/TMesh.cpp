#include "TMesh.h"

#include "PCH/PCH.h"

namespace gam
{
/************************* Triangulated Mesh **************************/

Mesh TMesh::mesh(bool curvature, bool remove_infinite) const
{
	Mesh mesh(GL_TRIANGLES);

	for(index_t i = 0; i < vertex_count(); ++i)
	{
		if(i == 0)
			mesh.color(White());
		else
			mesh.color(Red());
		if(m_normals.size() > 0)
		{
			mesh.normal(m_normals[i]);
		}
		if(curvature && m_curvature.size() > 0)
		{
			mesh.texcoord({ m_curvature[i], m_curvature[i] });
		}
		else if(m_values.size() > 0)
		{
			mesh.texcoord({ m_values[i], m_values[i] });
		}
		mesh.vertex(m_vertices[i].as_point());
	}

	for(index_t i = 0; i < face_count(); ++i)
	{
		if(remove_infinite && is_infinite_face(i))
			continue;
		mesh.triangle(m_faces[i][0], m_faces[i][1], m_faces[i][2]);
	}

	return mesh;
}

void TMesh::vertex_value(index_t i_vertex, scalar_t v)
{
	assert(i_vertex < vertex_count());
	m_values[i_vertex] = v;
}

scalar_t TMesh::vertex_value(index_t i_vertex) const
{
	assert(i_vertex < vertex_count());
	return m_values[i_vertex];
}

void gam::TMesh::load_off(const std::string& off_file)
{
	m_vertices.clear();
	m_faces.clear();
	m_normals.clear();
	m_curvature.clear();
	m_values.clear();

	std::ifstream file(std::string(OFF_DIR) + off_file);
	// Checking if we can read the file
	if(!file.is_open())
	{
		utils::error("in [load_off] Couldn't open this file: ", off_file);
		exit(1);
	}
	// Checking file type
	std::string type;
	file >> type;
	if(type != "OFF")
	{
		utils::error("in [load_off] The format of the provided file must be OFF: ", off_file);
		exit(1);
	}
	// Retrieving number of vertices / faces
	index_t nVertices, nFaces, nEdges;
	file >> nVertices >> nFaces >> nEdges;
	m_vertices.resize(nVertices);
	m_faces.resize(nFaces);
	m_values.resize(nVertices, 0.);
	m_curvature.resize(nVertices, 0.);
	m_normals.resize(nVertices, Vector(0., 0., 1.));

	Point pmin, pmax;
	// Initializing position of vertices
	for(index_t i = 0; i < nVertices; ++i)
	{
		Vertex vertex;
		file >> vertex.X >> vertex.Y >> vertex.Z;
		m_vertices[i] = vertex;
	}

	std::unordered_map<std::pair<index_t, index_t>, std::pair<index_t, int>, HashEdgePair> map;
	for(index_t i = 0; i < nFaces; ++i)
	{
		index_t v0, v1, v2;
		file >> nVertices >> v0 >> v1 >> v2;

		// Set FaceIndex for each vertex if it's not already the case
		if(m_vertices[v0].FaceIndex == -1)
			m_vertices[v0].FaceIndex = i;
		if(m_vertices[v1].FaceIndex == -1)
			m_vertices[v1].FaceIndex = i;
		if(m_vertices[v2].FaceIndex == -1)
			m_vertices[v2].FaceIndex = i;
		// Set vertex indices of the face i
		m_faces[i][0] = v0;
		assert(m_faces[i][0] != -1);
		m_faces[i][1] = v1;
		assert(m_faces[i][1] != -1);
		m_faces[i][2] = v2;
		assert(m_faces[i][2] != -1);
		// Set neighboring faces using the edges
		// Edge v0-v1
		if(map.find({ v0, v1 }) == map.end() && map.find({ v1, v0 }) == map.end())
		{
			map[{ v0, v1 }] = { i, 2 };
		}
		else // The edge with v0 and v1 is already registered in map
		{
			auto [FaceIndex, EdgeIndex] = map[{ v1, v0 }];
			m_faces[FaceIndex](EdgeIndex) = i;
			m_faces[i](2) = FaceIndex;
		}
		// Edge v1-v2
		if(map.find({ v1, v2 }) == map.end() && map.find({ v2, v1 }) == map.end())
		{
			map[{ v1, v2 }] = { i, 0 };
		}
		else // The edge with v1 and v2 is already registered in map
		{
			auto [FaceIndex, EdgeIndex] = map[{ v2, v1 }];
			m_faces[FaceIndex](EdgeIndex) = i;
			m_faces[i](0) = FaceIndex;
		}
		// Edge v2-v0
		if(map.find({ v2, v0 }) == map.end() && map.find({ v0, v2 }) == map.end())
		{
			map[{ v2, v0 }] = { i, 1 };
		}
		else // The edge with v2 and v0 is already registered in map
		{
			auto [FaceIndex, EdgeIndex] = map[{ v0, v2 }];
			m_faces[FaceIndex](EdgeIndex) = i;
			m_faces[i](1) = FaceIndex;
		}
	}

#ifndef NDEBUG
	integrity_check();
	utils::status("[load_off] Integrity_check passed");
	utils::status("TMesh successfully loaded");
#endif
	file.close();
}

void TMesh::save_obj(const std::string& obj_file, bool use_curvature, bool remove_inf)
{
	std::ofstream file(std::string(OBJ_DIR) + obj_file);
	file << "OBJ" << "\n";

	int f_count = face_count();
	int v_count = vertex_count();
	if(remove_inf)
	{
		v_count--;
		for(const auto& f : m_faces)
		{
			if(is_infinite_face(f))
				f_count--;
		}
	}

	file << v_count << " " << f_count << " " << 0 << "\n";

	// Save vertices position
	for(index_t i = remove_inf ? 1 : 0; i < vertex_count(); ++i)
	{
		auto v = m_vertices[i];
		file << "v " << v.X << " " << v.Y << " " << v.Z << "\n";
	}

	if(!remove_inf)
	{
		// Save textcoords
		auto& data = use_curvature ? m_curvature : m_values;
		for(auto v : data)
		{
			file << "vt " << v << " " << v << "\n";
		}

		// Save vertices normals
		for(auto n : m_normals)
		{
			file << "vn " << n.x << " " << n.y << " " << n.z << "\n";
		}
	}

	// Save topology
	for(const auto& f : m_faces)
	{
		if(remove_inf && is_infinite_face(f))
			continue;
		file << "f ";
		if(remove_inf)
		{
			file << (f[0] + 0) << "/" << (f[0] + 0) << "/" << (f[0] + 0) << " ";
			file << (f[1] + 0) << "/" << (f[1] + 0) << "/" << (f[1] + 0) << " ";
			file << (f[2] + 0) << "/" << (f[2] + 0) << "/" << (f[2] + 0) << "\n";
		}
		else
		{
			file << (f[0] + 1) << "/" << (f[0] + 1) << "/" << (f[0] + 1) << " ";
			file << (f[1] + 1) << "/" << (f[1] + 1) << "/" << (f[1] + 1) << " ";
			file << (f[2] + 1) << "/" << (f[2] + 1) << "/" << (f[2] + 1) << "\n";
		}
	}
	file.close();
	utils::status("File ", obj_file, " successfully saved in data/obj");
}

void TMesh::save_off(const std::string& off_file, bool remove_inf)
{
	std::ofstream file(std::string(OFF_DIR) + off_file);
	file << "OFF" << "\n";

	int f_count = face_count();
	int v_count = vertex_count();
	if(remove_inf)
	{
		v_count--;
		for(const auto& f : m_faces)
		{
			if(is_infinite_face(f))
				f_count--;
		}
	}

	file << v_count << " " << f_count << " " << 0 << "\n";

	// Save vertices position
	for(index_t i = remove_inf ? 1 : 0; i < vertex_count(); ++i)
	{
		auto v = m_vertices[i];
		file << v.X << " " << v.Y << " " << v.Z << "\n";
	}

	// Save topology
	for(const auto& f : m_faces)
	{
		if(remove_inf && is_infinite_face(f))
			continue;
		file << "3 ";
		if(remove_inf)
		{
			file << (f[0] - 1) << " " << (f[1] - 1) << " " << (f[2] - 1) << "\n";
		}
		else
		{
			file << (f[0] - 0) << " " << (f[1] - 0) << " " << (f[2] - 0) << "\n";
		}
	}
	file.close();
	utils::status("File ", off_file, " successfully saved in data/off");
}

index_t TMesh::local_index(index_t i_vertex, index_t i_face) const
{
	assert(i_face < face_count());
	assert(i_vertex < vertex_count());

	int localIndex = 0;
	while(m_faces[i_face].VertexIndices[localIndex] != static_cast<int32_t>(i_vertex) && localIndex < 3)
		localIndex++;
	return localIndex;
}

void TMesh::print_neighboring_faces_of_face(index_t i_face) const
{
	assert(i_face < face_count());

	int i = 0;
	utils::message("--Neighboring faces of face ", i_face, "--");
	for(auto face : m_faces[i_face].NeighborIndices)
	{
		utils::message("Neighboring face ", i++, ": ", face);
	}
	std::cout << std::endl;
}

void TMesh::print_neighboring_faces_of_vertex(index_t i_vertex) const
{
	assert(i_vertex < face_count());

	auto& faceStartIndex = m_vertices[i_vertex].FaceIndex;
	utils::message("--Neighboring faces of vertex ", i_vertex, "--");
	int i = 0;
	utils::message("Neighboring face ", i++, ": ", faceStartIndex);
	index_t localVertexIndex = local_index(i_vertex, faceStartIndex);
	int32_t currentFaceIndex = m_faces[faceStartIndex].NeighborIndices[IndexHelpers::Next[localVertexIndex]];
	while(currentFaceIndex != faceStartIndex)
	{
		utils::message("Neighboring face ", i++, ": ", currentFaceIndex);
		localVertexIndex = local_index(i_vertex, currentFaceIndex);
		currentFaceIndex = m_faces[currentFaceIndex].NeighborIndices[IndexHelpers::Next[localVertexIndex]];
	}
	std::cout << std::endl;
}

std::vector<index_t> TMesh::neighboring_faces_of_face(index_t i_face) const
{
	assert(i_face < face_count());

	std::vector<index_t> neighbors;
	for(auto face : m_faces[i_face].NeighborIndices)
	{
		neighbors.emplace_back(face);
	}
	return neighbors;
}

std::vector<index_t> TMesh::neighboring_faces_of_vertex(index_t i_vertex) const
{
	assert(i_vertex < vertex_count());

	std::vector<index_t> neighbors;
	neighbors.emplace_back(m_vertices[i_vertex].FaceIndex);
	index_t localVertexIndex = local_index(i_vertex, neighbors[0]);
	index_t currentFaceIndex = m_faces[neighbors[0]].NeighborIndices[(localVertexIndex + 1) % 3];
	while(currentFaceIndex != neighbors[0])
	{
		neighbors.emplace_back(currentFaceIndex);
		localVertexIndex = local_index(i_vertex, currentFaceIndex);
		currentFaceIndex = m_faces[currentFaceIndex].NeighborIndices[(localVertexIndex + 1) % 3];
	}

	return neighbors;
}

std::vector<index_t> TMesh::neighboring_vertices_of_vertex(index_t i_vertex) const
{
	assert(i_vertex < vertex_count());
	assert(m_vertices[i_vertex].FaceIndex != -1);

	std::vector<index_t> neighbors;
	index_t startFaceIndex = m_vertices[i_vertex].FaceIndex;
	index_t currentFaceIndex = startFaceIndex;
	index_t localIndex = local_index(i_vertex, currentFaceIndex);
	index_t nextVertexLocalIndex = (localIndex + 1) % 3;
	index_t nextVertexGlobalIndex = m_faces[currentFaceIndex].VertexIndices[nextVertexLocalIndex];
	neighbors.emplace_back(nextVertexGlobalIndex);
	currentFaceIndex = m_faces[currentFaceIndex].NeighborIndices[nextVertexLocalIndex];
	while(currentFaceIndex != startFaceIndex)
	{
		localIndex = local_index(i_vertex, currentFaceIndex);
		nextVertexLocalIndex = (localIndex + 1) % 3;
		nextVertexGlobalIndex = m_faces[currentFaceIndex].VertexIndices[nextVertexLocalIndex];
		neighbors.emplace_back(nextVertexGlobalIndex);
		currentFaceIndex = m_faces[currentFaceIndex].NeighborIndices[nextVertexLocalIndex];
	}

	return neighbors;
}

scalar_t TMesh::face_area(index_t i_face) const
{
	assert(i_face < face_count());

	const auto& f = m_faces[i_face];
	const Vertex& v0 = m_vertices[f.VertexIndices[0]];
	const Vertex& v1 = m_vertices[f.VertexIndices[1]];
	const Vertex& v2 = m_vertices[f.VertexIndices[2]];
	Vector v0v1(v0.as_point(), v1.as_point());
	Vector v0v2(v0.as_point(), v2.as_point());
	Vector n = cross(v0v1, v0v2);

	return 0.5 * length(n);
}

scalar_t TMesh::patch_area(index_t i_vertex) const
{
	assert(i_vertex < vertex_count());
	auto iFaces = neighboring_faces_of_vertex(i_vertex);
	scalar_t area = 0.f;
	for(auto i_face : iFaces)
	{
		area += face_area(i_face);
	}
	return area / 3;
}

void TMesh::laplacian()
{
	assert(m_values.size() == m_vertices.size());
	for(index_t i = 1; i < m_vertices.size(); ++i)
	{
		m_values[i] = laplacian(i);
	}
}

Vector TMesh::face_normal(index_t i_face) const
{
	assert(i_face < face_count());

	auto faceVertices = m_faces[i_face].VertexIndices;

	Vector u(m_vertices[faceVertices[0]].as_point(), m_vertices[faceVertices[1]].as_point());
	Vector v(m_vertices[faceVertices[0]].as_point(), m_vertices[faceVertices[2]].as_point());

	Vector faceNormal = cross(u, v);

	return normalize(faceNormal);
}

void TMesh::smooth_normals()
{
	assert(m_normals.size() == m_vertices.size());

	for(index_t i = 0; i < m_vertices.size(); ++i)
	{
		Vector laplacian = laplacian_vector(i);

		index_t i_face = m_vertices[i].FaceIndex;
		Vector faceNormal = face_normal(i_face);

		Vector vertexNormal = laplacian;
		if(dot(faceNormal, laplacian) < 0)
		{
			vertexNormal = -vertexNormal;
		}

		m_curvature[i] = length(vertexNormal);
		m_normals[i] = vertexNormal;
	}
}

void TMesh::curvature()
{
	scalar_t maxCurv = *std::max_element(m_curvature.begin(), m_curvature.end());

	assert(maxCurv > 0 || maxCurv < 0);

	for(index_t i = 0; i < m_vertices.size(); ++i)
	{
		m_curvature[i] = m_curvature[i] / maxCurv;
	}
}

void TMesh::heat_diffusion(scalar_t delta_time)
{
	for(index_t i = 1; i < vertex_count(); ++i)
	{
		heat_diffusion(i, delta_time);
	}
}

void TMesh::heat_diffusion(index_t i_vertex, scalar_t delta_time)
{
	assert(i_vertex < vertex_count());

	m_values[i_vertex] += delta_time * laplacian(i_vertex);
}

void TMesh::insert_vertex(float x, float y, float z)
{
	insert_vertex({ x, y, z });
}

void TMesh::clear()
{
	m_vertices.clear();
	m_faces.clear();
	m_normals.clear();
	m_curvature.clear();
	m_values.clear();
}

scalar_t TMesh::laplacian(index_t i_vertex)
{
	assert(m_values.size() == m_vertices.size());

	scalar_t Lui = 0.;
	auto neighboringVertices = neighboring_vertices_of_vertex(i_vertex);
	auto numOfNeighbors = neighboringVertices.size();
	for(size_t j = 1; j <= numOfNeighbors; ++j)
	{
		index_t jVertex = neighboringVertices[j % numOfNeighbors];
		index_t prevJVertex = neighboringVertices[j - 1];
		index_t nextJVertex = neighboringVertices[(j + 1) % numOfNeighbors];

		// Cotangente
		Vector v(m_vertices[prevJVertex].as_point(), m_vertices[jVertex].as_point());
		Vector w(m_vertices[prevJVertex].as_point(), m_vertices[i_vertex].as_point());
		scalar_t cotAlpha = cotan(v, w);
		v = Vector(m_vertices[nextJVertex].as_point(), m_vertices[i_vertex].as_point());
		w = Vector(m_vertices[nextJVertex].as_point(), m_vertices[jVertex].as_point());
		scalar_t cotBeta = cotan(v, w);
		scalar_t& ui = m_values[i_vertex];
		scalar_t& uj = m_values[jVertex];

		Lui += (cotAlpha + cotBeta) * (uj - ui);
	}
	return Lui / (2. * patch_area(i_vertex));
}

Vector TMesh::laplacian_vector(index_t i_vertex)
{
	assert(m_values.size() == m_vertices.size());
	Vector laplacian;
	auto neighboringVertices = neighboring_vertices_of_vertex(i_vertex);
	size_t numOfNeighbors = neighboringVertices.size();
	for(size_t j = 1; j <= numOfNeighbors; ++j)
	{
		index_t jVertex = neighboringVertices[j % numOfNeighbors];
		index_t prevJVertex = neighboringVertices[j - 1];
		index_t nextJVertex = neighboringVertices[(j + 1) % numOfNeighbors];

		// Cotangente
		Vector pj(m_vertices[prevJVertex].as_point(), m_vertices[jVertex].as_point());
		Vector pi(m_vertices[prevJVertex].as_point(), m_vertices[i_vertex].as_point());
		scalar_t cotAlpha = cotan(pj, pi);
		Vector ni = Vector(m_vertices[nextJVertex].as_point(), m_vertices[i_vertex].as_point());
		Vector nj = Vector(m_vertices[nextJVertex].as_point(), m_vertices[jVertex].as_point());
		scalar_t cotBeta = cotan(ni, nj);
		auto& vi = m_vertices[i_vertex];
		auto& vj = m_vertices[jVertex];
		Vector ui = Vector(vi.X, vi.Y, vi.Z);
		Vector uj = Vector(vj.X, vj.Y, vj.Z);

		laplacian.x += (cotAlpha + cotBeta) * (uj.x - ui.x);
		laplacian.y += (cotAlpha + cotBeta) * (uj.y - ui.y);
		laplacian.z += (cotAlpha + cotBeta) * (uj.z - ui.z);
	}
	laplacian = laplacian / (2. * patch_area(i_vertex));

	return laplacian;
}

void TMesh::insert_vertex(const Point& p)
{
	auto loc = locate_triangle(p);
	bool found = loc.first;
	int i_face = loc.second.first;
	int i_edge = loc.second.second;

	if(found) // p is in a face
	{
		if(i_edge >= 0)
		{
			edge_split(p, i_face, i_edge);
		}
		else
		{
			triangle_split(p, i_face);
		}
	}
	else // point is outside every faces
	{
		insert_outside(p, i_face);
	}

	lawson(vertex_count() - 1);
}

void TMesh::lawson(index_t i_vertex)
{
	auto faces = neighboring_faces_of_vertex(i_vertex);

	std::stack<index_t> to_check;

	for(auto i_face : faces)
		to_check.push(i_face);

	while(!to_check.empty())
	{
		index_t i_face0 = to_check.top();
		to_check.pop();
		if(is_infinite_face(i_face0))
			continue;

		Face face0 = m_faces[i_face0];
		index_t i_edge0 = local_index(i_vertex, i_face0);
		index_t i_face1 = face0(i_edge0);
		if(is_infinite_face(i_face1))
			continue;

		Face face1 = m_faces[i_face1];
		index_t i_edge1 = face1.get_edge(i_face0);

		Vertex p = m_vertices[face1[i_edge1]];
		Vertex a = m_vertices[face0[0]];
		Vertex b = m_vertices[face0[1]];
		Vertex c = m_vertices[face0[2]];
		if(in_circle(p.as_point(), a.as_point(), b.as_point(), c.as_point()))
		{
			flip_edge(i_face0, i_edge0);
			to_check.push(i_face0);
			to_check.push(i_face1);
		}
	}

	// #ifndef NDEBUG
	//     integrity_check();
	//     utils::status("[lawson] Integrity_check passed");
	// #endif
}

void TMesh::slide_triangle(index_t i_face)
{
	assert(i_face < face_count());

	if(m_faces[i_face][1] == 0)
	{
		m_faces[i_face].slide_vertices_left();
	}
	else if(m_faces[i_face][2] == 0)
	{
		m_faces[i_face].slide_vertices_right();
	}
}

bool TMesh::is_infinite_face(index_t i_face) const
{
	return is_infinite_face(m_faces[i_face]);
}

bool TMesh::is_infinite_face(Face face) const
{
	return face[0] == 0 || face[1] == 0 || face[2] == 0;
}

std::pair<bool, std::pair<int32_t, int8_t>> TMesh::locate_triangle(const Point& p) const
{
	int i_face = 0;
	int i_edge = 0;

	int i_edge_to_avoid = -1;
	int i_previous_face = -1;

	bool f_is_inf = false;
	bool p_in_f = false;
	int o;
	do
	{
		if(is_infinite_face(i_face)) // if its an infinite face
		{
			f_is_inf = true;
			continue;
		}

		if(i_edge_to_avoid != -1)
		{
			i_edge = (i_edge_to_avoid + 1) % 3;
		}
		else
		{
			i_edge_to_avoid = i_edge;
		}

		bool p_intersect_f = false;
		int tmp_i_edge = -1;
		Point a = m_vertices[m_faces[i_face][(i_edge + 2) % 3]].as_point();
		Point b = m_vertices[m_faces[i_face][(i_edge + 1) % 3]].as_point();
		while(!p_in_f && (o = orientation(a, b, p)) < 1) // clockwise or on edge
		{
			if(o == 0)
			{
				p_intersect_f = true;
				tmp_i_edge = i_edge;
			}
			i_edge = (i_edge + 1) % 3;
			if(i_edge == i_edge_to_avoid)
			{
				if(p_intersect_f)
				{
					i_edge = tmp_i_edge;
				}
				else
				{
					i_edge = -1;
				}
				p_in_f = true;
				continue;
			}

			a = m_vertices[m_faces[i_face][(i_edge + 2) % 3]].as_point();
			b = m_vertices[m_faces[i_face][(i_edge + 1) % 3]].as_point();
		}

		if(p_in_f)
			continue;

		i_previous_face = i_face;
		i_face = m_faces[i_face](i_edge);
		i_edge_to_avoid = m_faces[i_face].get_edge(i_previous_face);
	} while(!p_in_f && !f_is_inf);

	return { !f_is_inf, { i_face, i_edge } };
}

void TMesh::triangle_split(const Point& p, index_t i_face)
{
	auto face = m_faces[i_face];

	int i_vertex = vertex_count();

	m_vertices.emplace_back(p, i_face);

	index_t i_face2 = face_count();
	index_t i_face3 = face_count() + 1;

	m_faces[i_face][2] = i_vertex;
	m_faces[i_face](0) = i_face2;
	m_faces[i_face](1) = i_face3;

	m_faces.emplace_back(face[1], face[2], i_vertex, i_face3, i_face, face(0));
	m_faces.emplace_back(face[2], face[0], i_vertex, i_face, i_face2, face(1));

	m_vertices[face[2]].FaceIndex = i_face2;

	m_faces[face(0)].change_neighbor(i_face, i_face2);
	m_faces[face(1)].change_neighbor(i_face, i_face3);

#ifdef DEBUG
	integrity_check();
	utils::status("[triangle_split] Integrity_check passed");
#endif
}

void TMesh::edge_split(const Point& p, index_t i_face0, index_t i_edge0)
{
	index_t i_vertex = vertex_count();
	m_vertices.emplace_back(p, i_face0);

	index_t i_face2 = face_count();
	index_t i_face3 = face_count() + 1;

	index_t i_face1 = m_faces[i_face0](i_edge0);
	index_t i_edge1 = m_faces[i_face1].get_edge(i_face0);

	Face face0 = m_faces[i_face0];
	Face face1 = m_faces[i_face1];

	m_faces[i_face0].set_vertex_indices(face0[i_edge0], face0[(i_edge0 + 1) % 3], i_vertex);
	m_faces[i_face0].set_neighbor_indices(face0(i_edge0), i_face3, face0((i_edge0 + 2) % 3));

	m_faces[i_face1].set_vertex_indices(face1[i_edge1], i_vertex, face1[(i_edge1 + 2) % 3]);
	m_faces[i_face1].set_neighbor_indices(face1(i_edge1), face1((i_edge1 + 1) % 3), i_face2);

	m_faces.emplace_back(face1[i_edge1], face1[(i_edge1 + 1) % 3], i_vertex, i_face3, i_face1, face1((i_edge1 + 2) % 3));
	m_faces.emplace_back(face0[i_edge0], i_vertex, face0[(i_edge0 + 2) % 3], i_face2, face0((i_edge0 + 1) % 3), i_face0);

	m_vertices[face0[(i_edge0 + 2) % 3]].FaceIndex = i_face2;

	m_faces[face0((i_edge0 + 1) % 3)].change_neighbor(i_face0, i_face3);
	m_faces[face1((i_edge1 + 2) % 3)].change_neighbor(i_face1, i_face2);

#ifdef DEBUG
	integrity_check();
	utils::status("[edge_split] Integrity_check passed");
#endif
}

void TMesh::check_orientation(Face& face)
{
	Point a = m_vertices[face[0]].as_point();
	Point b = m_vertices[face[1]].as_point();
	Point c = m_vertices[face[2]].as_point();
	if(orientation(a, b, c) != 1)
	{
		index_t tmp = face[1];
		face[1] = face[2];
		face[2] = tmp;
	}
}

void TMesh::insert_outside(const Point& p, index_t i_face)
{
	auto nf = neighboring_faces_of_vertex(0);

	int itf = std::find(nf.begin(), nf.end(), i_face) - nf.begin();

	triangle_split(p, i_face);

	slide_triangle(face_count() - 1); // we check the last added face.

	// check right
	int i = (itf - 1 + nf.size()) % nf.size();
	Point a = m_vertices[m_faces[nf[i]][1]].as_point();
	Point b = m_vertices[m_faces[nf[i]][2]].as_point();
	while(orientation(a, b, p) == 1)
	{
		flip_edge(nf[i], 1);
		i = (i - 1 + nf.size()) % nf.size();
		a = m_vertices[m_faces[nf[i]][1]].as_point();
		b = m_vertices[m_faces[nf[i]][2]].as_point();
	}

	i = (itf + 1) % nf.size();
	a = m_vertices[m_faces[nf[i]][1]].as_point();
	b = m_vertices[m_faces[nf[i]][2]].as_point();
	while(orientation(a, b, p) == 1)
	{
		flip_edge(nf[i], 2);
		i = (i + 1) % nf.size();
		a = m_vertices[m_faces[nf[i]][1]].as_point();
		b = m_vertices[m_faces[nf[i]][2]].as_point();
	}
}

void TMesh::flip_edge(index_t i_face0, index_t i_edge0)
{
	index_t i_face1 = m_faces[i_face0](i_edge0);
	index_t i_edge1 = m_faces[i_face1].get_edge(i_face0);

	Face face0 = m_faces[i_face0];
	Face face1 = m_faces[i_face1];

	m_faces[i_face0][0] = face0[(i_edge0 + 1) % 3];
	m_faces[i_face0][1] = face1[i_edge1];
	m_faces[i_face0][2] = face0[i_edge0];
	m_faces[i_face0](0) = i_face1;
	m_faces[i_face0](1) = face0((i_edge0 + 2) % 3);
	m_faces[i_face0](2) = face1((i_edge1 + 1) % 3);

	m_faces[i_face1][0] = face1[(i_edge1 + 1) % 3];
	m_faces[i_face1][1] = face0[i_edge0];
	m_faces[i_face1][2] = face1[i_edge1];
	m_faces[i_face1](0) = i_face0;
	m_faces[i_face1](1) = face1((i_edge1 + 2) % 3);
	m_faces[i_face1](2) = face0((i_edge0 + 1) % 3);

	m_vertices[face0[(i_edge0 + 1) % 3]].FaceIndex = i_face0;
	m_vertices[face1[(i_edge1 + 1) % 3]].FaceIndex = i_face1;

	m_faces[face0((i_edge0 + 1) % 3)].change_neighbor(i_face0, i_face1);
	m_faces[face1((i_edge1 + 1) % 3)].change_neighbor(i_face1, i_face0);

#ifdef DEBUG
	integrity_check();
	utils::status("[flip_edge] Integrity_check passed");
#endif
}

void TMesh::insert_vertices(const std::vector<Point>& points, int point_count)
{
	assert(points.size() >= 3);

	clear();

	if(point_count == -1)
		point_count = points.size();

	m_values.reserve(points.size());
	for(const auto& p : points)
	{
		m_values.emplace_back(p.z);
	}

	m_vertices.reserve(points.size() + 1);

	m_vertices.emplace_back(0., 0., -1., 1);

	m_vertices.emplace_back(points[0].x, points[0].y, 0, 0);
	m_vertices.emplace_back(points[1].x, points[1].y, 0, 0);
	m_vertices.emplace_back(points[2].x, points[2].y, 0, 0);

	m_faces.emplace_back(1, 2, 3, 2, 3, 1);
	check_orientation(m_faces[0]);

	Face face0 = m_faces[0];

	m_faces.emplace_back(0, face0[1], face0[0], 0, face0(1), face0(0));
	m_faces.emplace_back(0, face0[2], face0[1], 0, face0(2), face0(1));
	m_faces.emplace_back(0, face0[0], face0[2], 0, face0(0), face0(2));

	for(int i = 3; i < point_count; ++i)
	{
		insert_vertex(points[i].x, points[i].y, 0.0);
	}

#ifndef NDEBUG
	integrity_check();
	utils::status("[insert_vertices] Integrity_check passed");
	delaunay_check();
	utils::status("[insert_vertices] Delaunay_check passed");
#endif

	for(index_t i = 1; i < vertex_count(); ++i)
	{
		m_vertices[i].Z = m_values[i - 1];
	}
}

void TMesh::delaunay_check() const
{
	for(index_t i_face = 0; i_face < face_count(); ++i_face)
	{
		if(is_infinite_face(i_face))
			continue;
		Face face = m_faces[i_face];
		Point a = m_vertices[face[0]].as_point();
		Point b = m_vertices[face[1]].as_point();
		Point c = m_vertices[face[2]].as_point();

		for(auto n : face.NeighborIndices)
		{
			if(is_infinite_face(n))
				continue;
			int i_edge = m_faces[n].get_edge(i_face);
			Point p = m_vertices[m_faces[n][i_edge]].as_point();
			assert(!in_circle(p, a, b, c));
		}
	}
}

void TMesh::integrity_check() const
{
	// Check the integrity of the vertices of the mesh.
	for(int32_t i = 0; i < static_cast<int32_t>(vertex_count()); ++i)
	{
		int i_face = m_vertices[i].FaceIndex;
		auto face = m_faces[i_face];
		assert(face[0] == i || face[1] == i || face[2] == i);
	}

	// Check the integrity of each neighbor of each face of the mesh.
	for(int32_t i = 0; i < static_cast<int32_t>(face_count()); ++i)
	{
		auto face = m_faces[i];
		if(is_infinite_face(i))
		{
			assert(face[0] == 0);
		}

		for(int32_t j = 0; j < 3; ++j)
		{
			auto neighbor = m_faces[face(j)];
			assert(neighbor(0) == i || neighbor(1) == i || neighbor(2) == i);
		}
	}
}
} // namespace gam
