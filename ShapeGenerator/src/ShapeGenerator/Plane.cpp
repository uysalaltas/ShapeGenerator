#include "Plane.h"
#include <ShapeGenerator.h>

namespace Shapes
{
	Plane::Plane(const float slice, const glm::vec3 color)
		: m_slice(slice)
		, m_color(color)
	{
		BuildVertices();
		BuildIndices();
	}

	std::vector<GLuint> Plane::ShapeIndices()
	{
		return m_indices;
	}

	std::vector<Vertex> Plane::ShapeVertices()
	{
		return m_vertices;
	}

	void Plane::BuildVertices()
	{
		float offset = (m_slice - 1) / 2;

		for (int row = 0; row < m_slice; row++)
		{
			for (int col = 0; col < m_slice; col++)
			{
				m_vertices.push_back(
					Vertex
					{
						glm::vec3((float)col - offset, (float)row - offset, 0.0f),
						m_color,
						glm::vec3(0, 0, 1)
					}
				);

				//std::cout << glm::to_string(glm::vec3((float)col, (float)row, 0.0f)) << std::endl;
			}
		}
	}

	void Plane::BuildIndices()
	{
		int ind1 = 0;
		int ind2 = 3;
		int ind3 = 1;

		for (int row = 0; row < m_slice - 1; row++) 
		{
			ind1 = 3 * row;

			std::cout << ind1 << std::endl;

			for (int col = 0; col < m_slice - 1; col++)
			{
				m_indices.push_back(col + ind1);
				m_indices.push_back(col + ind1 + ind2);
				m_indices.push_back(col + ind1 + ind3);

				m_indices.push_back(col + ind1 + ind2);
				m_indices.push_back(col + ind1 + ind3);
				m_indices.push_back(col + ind1 + ind2 + ind3);
			}
		}

		//for (int i = 0; i < m_indices.size(); i++)
		//{
		//	std::cout << m_indices[i] << std::endl;
		//}
	}
}
