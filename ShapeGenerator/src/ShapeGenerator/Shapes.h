#pragma once

#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

namespace Shapes
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 normal;
	};

	class Shapes
	{
	public:
		virtual ~Shapes() {};
		virtual std::vector<GLuint> ShapeIndices() = 0;
		virtual std::vector<Vertex> ShapeVertices() = 0;

		virtual std::vector<glm::vec3> ShapePositions() = 0;
		virtual std::vector<glm::vec3> ShapeColors() = 0;
		virtual std::vector<glm::vec3> ShapeNormals() = 0;

	protected:
		std::vector<Vertex> m_vertices = {};
		std::vector<GLuint> m_indices = {};
		std::vector<glm::vec3> m_positions = {};
		std::vector<glm::vec3> m_colors = {};
		std::vector<glm::vec3> m_normals = {};
	};
}

