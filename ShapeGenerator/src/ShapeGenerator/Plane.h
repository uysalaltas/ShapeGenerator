#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Plane : Shapes
	{
	public:
		Plane(const int height, const int width, const float sliceSize, const glm::vec3 color);
		std::vector<GLuint> ShapeIndices() override;
		std::vector<Vertex> ShapeVertices() override;

	private:
		void BuildVertices();
		void BuildIndices();

	private:
		const float m_sliceSize;
		const int m_width;
		const int m_height;
		const glm::vec3 m_color;

		std::vector<Vertex> m_vertices = {};
		std::vector<GLuint> m_indices = {};
		std::vector<glm::vec3> m_unitVertices;
	};
}