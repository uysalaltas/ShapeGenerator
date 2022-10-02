#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Cylinder : Shapes
	{
	public:
		Cylinder(const float size, const glm::vec3 color, const float height, const int sectorCount);
		std::vector<GLuint> ShapeIndices() override;
		std::vector<Vertex> ShapeVertices() override;

	private:
		void BuildVertexData();
		void BuildUnitCircleVertices();

		glm::vec3 ComputeFaceNormals(Vertex &v1, Vertex &v2, Vertex &v3);

	private:
		const float m_size;
		const float m_height;
		const int m_sectorCount;

		const glm::vec3 m_color;
		const int m_stackCount = 2;
		const float m_baseRadius = 1.0f;
		const float m_topRadius = 1.0f;
		
		unsigned int m_baseIndex;
		unsigned int m_topIndex;

		std::vector<Vertex> m_vertices = {};
		std::vector<GLuint> m_indices = {};
		std::vector<float> m_unitCircleVertices;
	};
}