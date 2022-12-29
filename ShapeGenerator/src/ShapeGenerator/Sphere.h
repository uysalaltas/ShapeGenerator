#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Sphere : public Shapes
	{
	public:
		Sphere(const float radius, const glm::vec3 color, const int sectorCount, const int sectorStep);
		inline std::vector<GLuint> ShapeIndices() override { return m_indices; }
		inline std::vector<Vertex> ShapeVertices() override { return m_vertices; }
		inline std::vector<glm::vec3> ShapePositions() override { return m_positions; }
		inline std::vector<glm::vec3> ShapeColors() override { return m_colors; }
		inline std::vector<glm::vec3> ShapeNormals() override { return m_normals; }

	private:
		void BuildVertexData();

	private:
		const float m_radius;
		const glm::vec3 m_color;
		const int m_sectorCount;
		const int m_stackCount;

		std::vector<glm::vec3> m_unitVertices;
	};
}