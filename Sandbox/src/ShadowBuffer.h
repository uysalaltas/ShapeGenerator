#pragma once
#include <glad/glad.h>
#include <iostream>

class ShadowBuffer
{
public:
	ShadowBuffer();
	~ShadowBuffer();
	void BindFrameBuffer() const;
	void UnbindFrameBuffer() const;
	void BindTexture() const;
	void UnbindTexture() const;

	unsigned int TextureID();

private:
	unsigned int m_RendererID;
	unsigned int m_depthTexture;
};