#include "Cylinder.h"

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
		// put side vertices to arrays
		for (int i = 0; i < 2; ++i)
		{
			float h = -m_height / 2.0f + i * m_height;           // z value; -h/2 to h/2
			float t = 1.0f - i;                              // vertical tex coord; 1 to 0

			for (int j = 0, k = 0; j <= m_sectorCount; ++j, k += 3)
			{
				float ux = m_unitCircleVertices[k];
				float uy = m_unitCircleVertices[k + 1];
				float uz = m_unitCircleVertices[k + 2];
				
				Vertex tmp;
				// position vector
				tmp.position.x = (ux * m_baseRadius);
				tmp.position.y = (uy * m_baseRadius);
				tmp.position.z = (h);

				tmp.color.x = 1.0;
				tmp.color.y = 1.0;
				tmp.color.z = 1.0;

				m_vertices.push_back(tmp);
			}
		}

		// the starting index for the base/top surface
		//NOTE: it is used for generating indices later
		m_baseIndex = (int)m_vertices.size();
		m_topIndex = m_baseIndex + m_sectorCount + 1; // include center vertex

		// put base and top vertices to arrays
		for (int i = 0; i < 2; ++i)
		{
			float h = -m_height / 2.0f + i * m_height;           // z value; -h/2 to h/2
			float nz = -1 + i * 2;                           // z value of normal; -1 to 1

			Vertex centerVertex;
			// center point
			centerVertex.position.x = 0.0f;
			centerVertex.position.y = 0.0f;
			centerVertex.position.z = h;

			centerVertex.color.x = 1.0;
			centerVertex.color.y = 1.0;
			centerVertex.color.z = 1.0;
			
			m_vertices.push_back(centerVertex);

			for (int j = 0, k = 0; j < m_sectorCount; ++j, k += 3)
			{
				float ux = m_unitCircleVertices[k];
				float uy = m_unitCircleVertices[k + 1];

				Vertex tmp;
				tmp.position.x = (ux * m_topRadius);
				tmp.position.y = (uy * m_topRadius);
				tmp.position.z = (h);

				tmp.color.x = 1.0;
				tmp.color.y = 1.0;
				tmp.color.z = 1.0;

				m_vertices.push_back(tmp);
			}
		}
	}

	void Cylinder::BuildIndices()
	{
		// generate CCW index list of cylinder triangles
		int k1 = 0;                         // 1st vertex index at base
		int k2 = m_sectorCount + 1;           // 1st vertex index at top

		// indices for the side surface
		for (int i = 0; i < m_sectorCount; ++i, ++k1, ++k2)
		{
			// 2 triangles per sector
			// k1 => k1+1 => k2
			m_indices.push_back(k1);
			m_indices.push_back(k1 + 1);
			m_indices.push_back(k2);

			// k2 => k1+1 => k2+1
			m_indices.push_back(k2);
			m_indices.push_back(k1 + 1);
			m_indices.push_back(k2 + 1);
		}

		// indices for the base surface
		//NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
		//      please see the previous code snippet
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
}
