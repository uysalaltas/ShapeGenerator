#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"

class ShadowMapping
{
public:
	ShadowMapping();
	~ShadowMapping();

	void RenderShadow(Shader& shader, glm::mat4& lightSpaceMatrix);
	void BindTexture() const;
	void UnbindFrameBuffer() const;
	void DebugShadow(Shader& shader);

private:
	void RenderQuad();

private:
	GLuint depthMapFBO;
	GLuint depthMapTexture;
	GLuint quadVAO = 0;
	GLuint quadVBO = 0;
	int shadowWidth = 2048;
	int shadowHeigth = 2048;
};