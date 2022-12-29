#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Plane : public Shapes
	{
	public:
		Plane(const int height, const int width, const float sliceSize, const glm::vec3 color);
		inline std::vector<GLuint> ShapeIndices() override { return m_indices; }
		inline std::vector<Vertex> ShapeVertices() override { return m_vertices; }
		inline std::vector<glm::vec3> ShapePositions() override { return m_positions; }
		inline std::vector<glm::vec3> ShapeColors() override { return m_colors; }
		inline std::vector<glm::vec3> ShapeNormals() override { return m_normals; }

	private:
		void BuildVertices();
		void BuildIndices();

	private:
		const float m_sliceSize;
		const int m_width;
		const int m_height;
		const glm::vec3 m_color;
		std::vector<glm::vec3> m_unitVertices;
	};
}