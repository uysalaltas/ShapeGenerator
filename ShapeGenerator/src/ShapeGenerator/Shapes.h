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
	};

	class Shapes
	{
	public:
		virtual ~Shapes() {};
		virtual std::vector<GLuint> ShapeIndices() = 0;
		virtual std::vector<Vertex> ShapeVertices() = 0;
	private:

	};
}

