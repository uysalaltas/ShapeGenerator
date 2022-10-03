#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Cube : public Shapes
	{
	public:
		Cube(const float size, const glm::vec3 color);
		std::vector<GLuint> ShapeIndices() override;
		std::vector<Vertex> ShapeVertices() override;

	private:
		void BuildVertexData();
		glm::vec3 ComputeFaceNormals(Vertex& v1, Vertex& v2, Vertex& v3);

	private:
		const float m_size;
		const glm::vec3 m_color;

		std::vector<glm::vec3> m_unitVertices;
		std::vector<Vertex> m_vertices = {};
		std::vector<GLuint> m_indices = {};

	};
}