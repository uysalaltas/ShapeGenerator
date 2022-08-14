#include "Cube.h"

namespace Shapes
{
	Cube::Cube(const float size, const glm::vec3 color)
		: m_size(size)
		, m_color(color)
	{

	}

	std::vector<GLuint> Cube::ShapeIndices()
	{
		return m_indices;
	}

	std::vector<Vertex> Cube::ShapeVertices()
	{
		return m_vertices;
	}

}
