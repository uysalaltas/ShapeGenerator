#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Cube : public Shapes
	{
	public:
		Cube(const float size, const glm::vec3 color);
		virtual std::vector<GLuint> ShapeIndices();
		virtual std::vector<Vertex> ShapeVertices();

	private:
		const float m_size;
		const glm::vec3 m_color;

		std::vector<Vertex> m_vertices =
		{
			Vertex{glm::vec3(0.0f, 0.0f, 1.0f) * m_size, m_color},
			Vertex{glm::vec3(1.0f, 0.0f, 1.0f) * m_size, m_color},
			Vertex{glm::vec3(1.0f, 1.0f, 1.0f) * m_size, m_color},
			Vertex{glm::vec3(0.0f, 1.0f, 1.0f) * m_size, m_color},
			Vertex{glm::vec3(0.0f, 0.0f, 0.0f) * m_size, m_color},
			Vertex{glm::vec3(1.0f, 0.0f, 0.0f) * m_size, m_color},
			Vertex{glm::vec3(1.0f, 1.0f, 0.0f) * m_size, m_color},
			Vertex{glm::vec3(0.0f, 1.0f, 0.0f) * m_size, m_color}
		};
		std::vector<GLuint> m_indices =
		{
			// front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3,
		};
	};
}