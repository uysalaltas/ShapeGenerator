#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Cube : public Shapes
	{
	public:
		Cube(const float size, const glm::vec3 color);
		inline std::vector<GLuint> ShapeIndices() override { return m_indices; }
		inline std::vector<Vertex> ShapeVertices() override { return m_vertices; }
		inline std::vector<glm::vec3> ShapePositions() override { return m_positions; }
		inline std::vector<glm::vec3> ShapeColors() override { return m_colors; }
		inline std::vector<glm::vec3> ShapeNormals() override { return m_normals; }

	private:
		void BuildVertexData();
		glm::vec3 ComputeFaceNormals(Vertex& v1, Vertex& v2, Vertex& v3);

	private:
		const float m_size;
		const glm::vec3 m_color;

		std::vector<glm::vec3> m_unitVertices;
	};
}