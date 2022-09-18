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
		camera = new Shapes::Camera(cameraPos, cameraCenter, cameraUp, (float)this->GetWindow().GetWidth(), (float)this->GetWindow().GetHeigth());

		va.Bind();
        vb = new VertexBuffer(cylender.ShapeVertices());
        ib = new IndexBuffer(cylender.ShapeIndices());
        va.AddBuffer(*vb, 0, 3, sizeof(Shapes::Vertex), (void*)0);
        va.AddBuffer(*vb, 1, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::color));
		va.Unbind();
		vb->Unbind();
		ib->Unbind();
	}

	~Sandbox()
	{
		delete vb;
		delete ib;
		delete camera;
	}

	void OnUpdate() override
	{
		camera->UpdateProjMatrix();

		model = glm::mat4(1.0f);
		view = camera->GetViewMatrix();
		projection = camera->GetProjMatrix();

		processInput();

		shaderBasic.Bind();
		shaderBasic.SetUniformMat4f("model", model);
		shaderBasic.SetUniformMat4f("view", view);
		shaderBasic.SetUniformMat4f("projection", projection);

		va.Bind();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, cylender.ShapeIndices().size(), GL_UNSIGNED_INT, 0);
	}

	void processInput()
	{
		currentMousePosClick.x = Teapot::Input::GetMouseX();
		currentMousePosClick.y = Teapot::Input::GetMouseY();

		if (Teapot::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
		{
			if (firstMouseClick)
			{
				lastMousePosRightClick.x = Teapot::Input::GetMouseX();
				lastMousePosRightClick.y = Teapot::Input::GetMouseY();
				firstMouseClick = false;
			}

			camera->ArcBallCamera((lastMousePosRightClick.x - Teapot::Input::GetMouseX()), (lastMousePosRightClick.y - Teapot::Input::GetMouseY()));
			lastMousePosRightClick.x = Teapot::Input::GetMouseX();
			lastMousePosRightClick.y = Teapot::Input::GetMouseY();
		}
		else
		{
			firstMouseClick = true;
		}
	}

public:
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;

private:
    VertexArray va;
    VertexBuffer* vb;
    IndexBuffer* ib;
	Shapes::Camera* camera;

	Shapes::Cube cube;
	Shapes::Cylinder cylender;
	Shader shaderBasic;

	glm::vec3 cameraPos = glm::vec3(5.0f, 5.0f, 5.0f);
	glm::vec3 cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
	float deltaTime = 0.0f;

	glm::vec2 lastMousePosRightClick = glm::vec2(0.0f, 0.0f);
	glm::vec2 currentMousePosClick = glm::vec2(0.0f, 0.0f);;
	bool firstMouseClick;
};

int main()
{
	Sandbox* s = new Sandbox();
	s->GetWindow().GetWidth();
	s->Run();
	s->OnUpdate();
	delete(s);
}