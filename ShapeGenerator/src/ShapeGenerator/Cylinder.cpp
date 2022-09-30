#include "Cylinder.h"
#include <glm/gtx/string_cast.hpp>

namespace Shapes
{
	Cylinder::Cylinder(const float size, const glm::vec3 color)
		: m_size(size)
		, m_color(color)
	{
		BuildUnitCircleVertices();
		BuildVertices();
		BuildIndices();
	}
	
	std::vector<GLuint> Cylinder::ShapeIndices()
	{
		return m_indices;
	}

	std::vector<Vertex> Cylinder::ShapeVertices()
	{
		return m_vertices;
	}

	void Cylinder::BuildVertices()
	{
		// ------------------------
		// SIDE
		// ------------------------

		std::vector<Vertex> tmp_vertices = {};

		for (int i = 0; i <= m_stackCount; ++i)
		{
			float h = -(m_height / 2.0f) + (float)i / m_stackCount * m_height;			// z value; -h/2 to h/2
			float radius = m_baseRadius + (float)i / m_stackCount * (m_topRadius - m_baseRadius);
			float t = 1.0f - (float)i / m_stackCount;									// vertical tex coord; 1 to 0

			for (int j = 0, k = 0; j <= m_sectorCount; ++j, k += 3)
			{
				float ux = m_unitCircleVertices[k];
				float uy = m_unitCircleVertices[k + 1];
				float uz = m_unitCircleVertices[k + 2];
				
				Vertex tmp;
				tmp.position.x = (ux * radius) * m_size;
				tmp.position.y = (uy * radius) * m_size;
				tmp.position.z = (h) * m_size;

				tmp.color = m_color;

				//tmp.normal.x = ux;
				//tmp.normal.y = uy;
				//tmp.normal.z = uz;

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

		// v2-v4 <== stack at i+1
		// | \ |
		// v1-v3 <== stack at i

		std::cout << m_vertices.size() << std::endl;

		for (i = 0; i < m_stackCount; ++i)
		{
			vi1 = i * (m_sectorCount + 1);
			vi2 = (i + 1) * (m_sectorCount + 1);

			for (j = 0; j < m_sectorCount; ++j, ++vi1, ++vi2)
			{
				//std::cout << " vi1 " << vi1 << " vi2 " << vi2 << std::endl;

				v1 = tmp_vertices[vi1];
				v2 = tmp_vertices[vi2];
				v3 = tmp_vertices[vi1 + 1];
				v4 = tmp_vertices[vi2 + 1];

				// compute a face normal of v1-v3-v2
				n = ComputeFaceNormals(v1, v3, v2);
				//std::cout << glm::to_string(n) << std::endl;
				v1.normal = n;
				v2.normal = n;
				v3.normal = n;
				v4.normal = n;

				m_vertices.push_back(v1);
				m_vertices.push_back(v2);
				m_vertices.push_back(v3);
				m_vertices.push_back(v4);

				// k1 => k1+1 => k2
				m_indices.push_back(index);
				m_indices.push_back(index + 2);
				m_indices.push_back(index + 1);

				// k2 => k1+1 => k2+1
				m_indices.push_back(index + 1);
				m_indices.push_back(index + 2);
				m_indices.push_back(index + 3);

				index += 4;
			}
		}

		//------------------------
		//BASE AND TOP
		//------------------------

		m_baseIndex = (int)m_vertices.size();
		m_topIndex = m_baseIndex + m_sectorCount + 1;

		for (int i = 0; i < 2; ++i)
		{
			float h = -m_height / 2.0f + i * m_height;			// z value; -h/2 to h/2
			float nz = -1 + i * 2;								// z value of normal; -1 to 1

			Vertex centerVertex;
			centerVertex.position.x = 0.0f;
			centerVertex.position.y = 0.0f;
			centerVertex.position.z = h * m_size;

			centerVertex.color = m_color;

			centerVertex.normal.x = 0;
			centerVertex.normal.y = 0;
			centerVertex.normal.z = nz;
			
			m_vertices.push_back(centerVertex);

			for (int j = 0, k = 0; j < m_sectorCount; ++j, k += 3)
			{
				float ux = m_unitCircleVertices[k];
				float uy = m_unitCircleVertices[k + 1];

				Vertex tmp;
				tmp.position.x = (ux * m_topRadius) * m_size;
				tmp.position.y = (uy * m_topRadius) * m_size;
				tmp.position.z = (h) * m_size;

				tmp.color = m_color;

				tmp.normal.x = 0;
				tmp.normal.y = 0;
				tmp.normal.z = nz;

				m_vertices.push_back(tmp);
			}
		}
	}

	void Cylinder::BuildIndices()
	{
		// indices for the base surface
		for (int i = 0, k = m_baseIndex + 1; i < m_sectorCount; ++i, ++k)
		{
			if (i < m_sectorCount - 1)
			{
				m_indices.push_back(m_baseIndex);
				m_indices.push_back(k + 1);
				m_indices.push_back(k);
			}
			else // last triangle
			{
				m_indices.push_back(m_baseIndex);
				m_indices.push_back(m_baseIndex + 1);
				m_indices.push_back(k);
			}
		}

		// indices for the top surface
		for (int i = 0, k = m_topIndex + 1; i < m_sectorCount; ++i, ++k)
		{
			if (i < m_sectorCount - 1)
			{
				m_indices.push_back(m_topIndex);
				m_indices.push_back(k);
				m_indices.push_back(k + 1);
			}
			else // last triangle
			{
				m_indices.push_back(m_topIndex);
				m_indices.push_back(k);
				m_indices.push_back(m_topIndex + 1);
			}
		}
	}

	void Cylinder::BuildUnitCircleVertices()
	{
		const float PI = acos(-1);
		float sectorStep = 2 * PI / m_sectorCount;
		float sectorAngle;  // radian

		for (int i = 0; i <= m_sectorCount; ++i)
		{
			sectorAngle = i * sectorStep;
			m_unitCircleVertices.push_back(cos(sectorAngle)); // x
			m_unitCircleVertices.push_back(sin(sectorAngle)); // y
			m_unitCircleVertices.push_back(0);                // z
		}
	}

	glm::vec3 Cylinder::ComputeFaceNormals(Vertex& v1, Vertex& v2, Vertex& v3)
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
