#include "ShapeGenerator.h"
#include "Teapot.h"
#include "ShapeGenerator/Cube.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Sandbox : public Teapot::Application
{
public:
	Sandbox() 
		: cube(0.5f, glm::vec3(1.0f, 1.0f, 1.0f))
		, cylender(0.5f, glm::vec3(1.0f, 1.0f, 1.0f))
		, shaderBasic("src/Basic.shader")
	{
		va.Bind();
        vb = new VertexBuffer(cylender.ShapeVertices());
        ib = new IndexBuffer(cylender.ShapeIndices());
        va.AddBuffer(*vb, 0, 3, sizeof(Shapes::Vertex), (void*)0);
        va.AddBuffer(*vb, 1, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::color));
		va.Unbind();
		vb->Unbind();
		ib->Unbind();

		projection = glm::perspective(glm::radians(45.0f), (float)this->GetWindow().GetWidth() / (float)this->GetWindow().GetHeigth(), 0.1f, 100.0f);
	}

	~Sandbox()
	{
		delete vb;
		delete ib;
	}

	void OnUpdate() override
	{
		processInput();

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		model = glm::mat4(1.0f);

		shaderBasic.Bind();
		shaderBasic.SetUniformMat4f("model", model);
		shaderBasic.SetUniformMat4f("view", view);
		shaderBasic.SetUniformMat4f("projection", projection);

		va.Bind();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, cylender.ShapeIndices().size(), GL_UNSIGNED_INT, 0);
	}

	void processInput()
	{
		float cameraSpeed = static_cast<float>(0.1);
		if (Teapot::Input::IsKeyPressed(TEA_KEY_W))
			cameraPos += cameraSpeed * cameraFront;
		if (Teapot::Input::IsKeyPressed(TEA_KEY_S))
			cameraPos -= cameraSpeed * cameraFront;
		if (Teapot::Input::IsKeyPressed(TEA_KEY_A))
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (Teapot::Input::IsKeyPressed(TEA_KEY_D))
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

public:
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;

private:
    VertexArray va;
    VertexBuffer* vb;
    IndexBuffer* ib;

	Shapes::Cube cube;
	Shapes::Cylinder cylender;
	Shader shaderBasic;

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float deltaTime = 0.0f;
};

int main()
{
	Sandbox* s = new Sandbox();
	s->GetWindow().GetWidth();
	s->Run();
	s->OnUpdate();
	delete(s);
}