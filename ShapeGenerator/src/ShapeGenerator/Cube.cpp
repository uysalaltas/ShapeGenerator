#include "Cube.h"
#include "ShapeGenerator/Utils.h"

namespace Shapes
{
	Cube::Cube(const float size, const glm::vec3 color)
		: m_size(size)
		, m_color(color)
	{
		BuildVertexData();
	}

	std::vector<GLuint> Cube::ShapeIndices()
	{
		return m_indices;
	}

	std::vector<Vertex> Cube::ShapeVertices()
	{
		return m_vertices;
	}

	void Cube::BuildVertexData()
	{
		m_unitVertices.push_back({  0.5f,  0.5f, 0.0f });
		m_unitVertices.push_back({ -0.5f,  0.5f, 0.0f });
		m_unitVertices.push_back({ -0.5f, -0.5f, 0.0f });
		m_unitVertices.push_back({  0.5f, -0.5f, 0.0f });
		m_unitVertices.push_back({  0.5f,  0.5f, 0.0f });

		// ------------------------
		// SIDE
		// ------------------------
		std::vector<Vertex> tmp_vertices = {};

		for (int i = 0; i <= 1; ++i)
		{
			float h = -1 + (float)i;
			for (int j = 0, k = 0; j <= 4; ++j, k += 3)
			{
				float ux = m_unitVertices[j].x;
				float uy = m_unitVertices[j].y;

				Vertex tmp;
				tmp.position.x = (ux) * m_size;
				tmp.position.y = (uy) * m_size;
				tmp.position.z = (h) * m_size;

				tmp.color = m_color;

				tmp_vertices.push_back(tmp);
			}
		}

		// ------------------------
		// FLAT NORMALS
		// ------------------------
		Vertex v1, v2, v3, v4;
		glm::vec3 n;
		int vi1, vi2, i, j;
		int index = 0;

		for (i = 0; i < 1; ++i)
		{
			vi1 = i * (4 + 1);
			vi2 = (i + 1) * (4 + 1);

			for (j = 0; j < 4; ++j, ++vi1, ++vi2)
			{
				v1 = tmp_vertices[vi1];
				v2 = tmp_vertices[vi2];
				v3 = tmp_vertices[vi1 + 1];
				v4 = tmp_vertices[vi2 + 1];

				n = Utils::ComputeFaceNormals(v1, v3, v2);

				v1.normal = n;
				v2.normal = n;
				v3.normal = n;
				v4.normal = n;

				m_vertices.push_back(v1);
				m_vertices.push_back(v2);
				m_vertices.push_back(v3);
				m_vertices.push_back(v4);

				m_indices.push_back(index);
				m_indices.push_back(index + 2);
				m_indices.push_back(index + 1);

				m_indices.push_back(index + 1);
				m_indices.push_back(index + 2);
				m_indices.push_back(index + 3);
				index += 4;
			}
		}
	
		//------------------------
		//BASE AND TOP
		//------------------------
		unsigned int m_baseIndex = (int)m_vertices.size();
		unsigned int m_topIndex = m_baseIndex + 4;

		for (int i = 0; i < 2; i++)
		{
			float h = -1 + (float)i;
			float nz = -1 + i * 2;

			for (int j = 0; j < 4; j++)
			{
				float ux = m_unitVertices[j].x;
				float uy = m_unitVertices[j].y;
				
				Vertex tmp;
				tmp.position.x = (ux) * m_size;
				tmp.position.y = (uy) * m_size;
				tmp.position.z = h * m_size;
				tmp.color = m_color;
				tmp.normal.x = 0;
				tmp.normal.y = 0;
				tmp.normal.z = nz;
				m_vertices.push_back(tmp);
			}
		}

		// indices for the base surface
		for (int i = 0, k = m_baseIndex; i < 2; ++i, ++k)
		{
			m_indices.push_back(m_baseIndex);
			m_indices.push_back(k + 2);
			m_indices.push_back(k + 1);
		}

		// indices for the top surface
		for (int i = 0, k = m_topIndex; i < 2; ++i, ++k)
		{
			m_indices.push_back(m_topIndex);
			m_indices.push_back(k + 2);
			m_indices.push_back(k + 1);
		}
	}

	glm::vec3 Cube::ComputeFaceNormals(Vertex& v1, Vertex& v2, Vertex& v3)
	{
		const float EPSILON = 0.000001f;

		glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
		float nx, ny, nz;

		// find 2 edge vectors: v1-v2, v1-v3
		float ex1 = v2.position.x - v1.position.x;
		float ey1 = v2.position.y - v1.position.y;
		float ez1 = v2.position.z - v1.position.z;
		float ex2 = v3.position.x - v1.position.x;
		float ey2 = v3.position.y - v1.position.y;
		float ez2 = v3.position.z - v1.position.z;

		// cross product: e1 x e2
		nx = ey1 * ez2 - ez1 * ey2;
		ny = ez1 * ex2 - ex1 * ez2;
		nz = ex1 * ey2 - ey1 * ex2;

		// normalize only if the length is > 0
		float length = sqrtf(nx * nx + ny * ny + nz * nz);
		if (length > EPSILON)
		{
			// normalize
			float lengthInv = 1.0f / length;
			normal.x = nx * lengthInv;
			normal.y = ny * lengthInv;
			normal.z = nz * lengthInv;
		}

		return normal;
	}

}
