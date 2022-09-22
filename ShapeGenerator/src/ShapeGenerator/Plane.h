#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Plane : Shapes
	{
	public:
		Plane(const float slice, const glm::vec3 color);
		std::vector<GLuint> ShapeIndices() override;
		std::vector<Vertex> ShapeVertices() override;

	private:
		void BuildVertices();
		void BuildIndices();

	private:
		const float m_slice;
		const glm::vec3 m_color;

		std::vector<Vertex> m_vertices = {};
		std::vector<GLuint> m_indices = {};
	};
}