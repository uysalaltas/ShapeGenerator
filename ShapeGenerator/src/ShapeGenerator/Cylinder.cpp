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
		std::vector<Vertex> tmpVertices;
		int i, j, k;
		float x, y, z, s, t, radius;

		for (i = 0; i <= m_stackCount; ++i)
		{
			z = -(m_height * 0.5f) + (float)i / m_stackCount * m_height;
			radius = m_baseRadius + (float)i / m_stackCount * (m_topRadius - m_baseRadius);

			for (j = 0, k = 0; j <= m_sectorCount; ++j, k += 3)
			{
				x = m_unitCircleVertices[k];
				y = m_unitCircleVertices[k + 1];

				Vertex vertex;
				vertex.position.x = x * radius;
				vertex.position.y = y * radius;
				vertex.position.z = z;
				vertex.color.x = 1.0;
				vertex.color.y = 1.0;
				vertex.color.z = 1.0;
				tmpVertices.push_back(vertex);
			}
		}

        Vertex v1, v2, v3, v4;      // 4 vertex positions v1, v2, v3, v4
        std::vector<float> n;       // 1 face normal
        int vi1, vi2;               // indices
        int index = 0;

        // v2-v4 <== stack at i+1
        // | \ |
        // v1-v3 <== stack at i
        for (i = 0; i < m_stackCount; ++i)
        {
            vi1 = i * (m_sectorCount + 1);            // index of tmpVertices
            vi2 = (i + 1) * (m_sectorCount + 1);

            for (j = 0; j < m_sectorCount; ++j, ++vi1, ++vi2)
            {
                v1 = tmpVertices[vi1];
                v2 = tmpVertices[vi2];
                v3 = tmpVertices[vi1 + 1];
                v4 = tmpVertices[vi2 + 1];

                // put quad vertices: v1-v2-v3-v4
                m_vertices.push_back(v1);
                m_vertices.push_back(v2);
                m_vertices.push_back(v3);
                m_vertices.push_back(v4);

                // put indices of a quad
                m_indices.push_back(index);
                m_indices.push_back(index + 2);
                m_indices.push_back(index + 1);

                m_indices.push_back(index + 1);
                m_indices.push_back(index + 2);
                m_indices.push_back(index + 3);

                //// vertical line per quad: v1-v2
                //lineIndices.push_back(index);
                //lineIndices.push_back(index + 1);
                //// horizontal line per quad: v2-v4
                //lineIndices.push_back(index + 1);
                //lineIndices.push_back(index + 3);
                //if (i == 0)
                //{
                //    lineIndices.push_back(index);
                //    lineIndices.push_back(index + 2);
                //}

                index += 4;     // for next
            }
        }

        // remember where the base index starts
        m_baseIndex = (unsigned int)m_indices.size();
        unsigned int baseVertexIndex = (unsigned int)m_vertices.size() / 3;

        // put vertices of base of cylinder
        z = -m_height * 0.5f;
        
        Vertex baseVertex;
        baseVertex.position = {0, 0, z};
        baseVertex.color = {1.0f, 1.0f, 1.0f};
        m_vertices.push_back(baseVertex);

        for (i = 0, j = 0; i < m_sectorCount; ++i, j += 3)
        {
            x = m_unitCircleVertices[j];
            y = m_unitCircleVertices[j + 1];
            Vertex tmpVertex;
            tmpVertex.position = { x * m_baseRadius, y * m_baseRadius, z };
            tmpVertex.color = { 1.0f, 1.0f, 1.0f };
        }

        // put indices for base
        for (i = 0, k = baseVertexIndex + 1; i < m_sectorCount; ++i, ++k)
        {
            if (i < m_sectorCount - 1)
            {
                m_indices.push_back(baseVertexIndex);
                m_indices.push_back(k + 1);
                m_indices.push_back(k);
            }
            else
            {
                m_indices.push_back(baseVertexIndex);
                m_indices.push_back(baseVertexIndex + 1);
                m_indices.push_back(k);
            }
        }

        // remember where the top index starts
        m_topIndex = (unsigned int)m_indices.size();
        unsigned int topVertexIndex = (unsigned int)m_vertices.size() / 3;

        // put vertices of top of cylinder
        z = m_height * 0.5f;
        Vertex topVertex;
        baseVertex.position = { 0, 0, z };
        baseVertex.color = { 1.0f, 1.0f, 1.0f };
        m_vertices.push_back(topVertex);

        for (i = 0, j = 0; i < m_sectorCount; ++i, j += 3)
        {
            x = m_unitCircleVertices[j];
            y = m_unitCircleVertices[j + 1];

            Vertex tmpVertex;
            tmpVertex.position = { x * m_topRadius, y * m_topRadius, z };
            tmpVertex.color = { 1.0f, 1.0f, 1.0f };
        }

        for (i = 0, k = topVertexIndex + 1; i < m_sectorCount; ++i, ++k)
        {
            if (i < m_sectorCount - 1)
            {
                m_indices.push_back(topVertexIndex);
                m_indices.push_back(k);
                m_indices.push_back(k + 1);
            }
            else
            {
                m_indices.push_back(topVertexIndex);
                m_indices.push_back(k);
                m_indices.push_back(topVertexIndex + 1);
            }
        }
	}

	void Cylinder::BuildIndices()
	{

	}

	void Cylinder::BuildUnitCircleVertices()
	{
		const float PI = acos(-1);
		float sectorStep = 2 * PI / m_sectorCount;
		float sectorAngle;  // radian

		std::vector<float>().swap(m_unitCircleVertices);
		for (int i = 0; i <= m_sectorCount; ++i)
		{
			sectorAngle = i * sectorStep;
			m_unitCircleVertices.push_back(cos(sectorAngle)); // x
			m_unitCircleVertices.push_back(sin(sectorAngle)); // y
			m_unitCircleVertices.push_back(0);                // z
		}
	}
}
