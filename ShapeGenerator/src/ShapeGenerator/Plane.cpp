#include "Plane.h"

namespace Shapes
{
	Plane::Plane(const int height, const int width, const float sliceSize, const glm::vec3 color)
		: m_height(height)
		, m_width(width)
		, m_sliceSize(sliceSize)
		, m_color(color)
	{
		BuildVertices();
		BuildIndices();
	}

	void Plane::BuildVertices()
	{
		m_unitVertices.push_back({ m_sliceSize,		m_sliceSize,	0.0f });
		m_unitVertices.push_back({ 0.0f,			m_sliceSize,	0.0f });
		m_unitVertices.push_back({ 0.0f,			0.0f,			0.0f });
		m_unitVertices.push_back({ m_sliceSize,		0.0f,			0.0f });

		int rowK = 0;
		int colK = 0;

		for (int row = 0; row < m_width; row++)
		{
			rowK = row * m_sliceSize;
			for (int col = 0; col < m_height; col++)
			{
				colK = col * m_sliceSize;
				for (int i = 0; i < 4; i++)
				{
					m_vertices.push_back(
						Vertex
						{
							glm::vec3{(m_unitVertices[i].x + colK), (m_unitVertices[i].y + rowK), m_unitVertices[i].z},
							m_color,
							glm::vec3(0, 0, 1)
						}
					);

					m_positions.push_back(glm::vec3{ (m_unitVertices[i].x + colK), (m_unitVertices[i].y + rowK), m_unitVertices[i].z });
					m_colors.push_back(m_color);
					m_normals.push_back(glm::vec3(0, 0, 1));
				}
			}
		}
	}

	void Plane::BuildIndices()
	{
		int ind1 = 0;
		int ind2 = 1;
		int ind3 = 2;
		int ind4 = 3;

		int k = 0;

		for (int row = 0; row < m_width; row++)
		{
			for (int col = 0; col < m_height; col++)
			{
				m_indices.push_back(ind1 + k);
				m_indices.push_back(ind3 + k);
				m_indices.push_back(ind2 + k);

				m_indices.push_back(ind1 + k);
				m_indices.push_back(ind4 + k);
				m_indices.push_back(ind3 + k);

				k += 4;
			}
		}
	}
}
