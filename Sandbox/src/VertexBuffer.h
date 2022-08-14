#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <ShapeGenerator.h>

class VertexBuffer
{
public:
	VertexBuffer(std::vector<Shapes::Vertex>& vertices);
	~VertexBuffer();
	void BufferDataModification(std::vector<Shapes::Vertex>& vertices);
	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;

};