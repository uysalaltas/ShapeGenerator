#include "ShapeGenerator.h"
#include "Teapot.h"
#include "ShapeGenerator/Cube.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShadowBuffer.h"
#include "Shader.h"

class Sandbox : public Teapot::Application
{
public:
	Sandbox()
		: cube(0.5f, glm::vec3(1.0f, 1.0f, 1.0f))
		, cylender(0.30f, glm::vec3(1.0f, 1.0f, 1.0f))
		, plane(3.0f, glm::vec3(0.78f, 0.95f, 1.0f))
		, shaderBasic("src/Basic.shader")
		, shaderDepthBasic("src/BasicDepth.shader")
	{
		camera = new Shapes::Camera(cameraPos, cameraCenter, cameraUp, (float)this->GetWindow().GetWidth(), (float)this->GetWindow().GetHeigth());

		va.Bind();
        vb = new VertexBuffer(cylender.ShapeVertices());
        ib = new IndexBuffer(cylender.ShapeIndices());
        va.AddBuffer(*vb, 0, 3, sizeof(Shapes::Vertex), (void*)0);
        va.AddBuffer(*vb, 1, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::color));
        va.AddBuffer(*vb, 2, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::normal));
		va.Unbind();
		vb->Unbind();
		ib->Unbind();

		vaPlane.Bind();
		vbPlane = new VertexBuffer(plane.ShapeVertices());
		ibPlane = new IndexBuffer(plane.ShapeIndices());
		vaPlane.AddBuffer(*vbPlane, 0, 3, sizeof(Shapes::Vertex), (void*)0);
		vaPlane.AddBuffer(*vbPlane, 1, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::color));
		vaPlane.AddBuffer(*vbPlane, 2, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::normal));
		vaPlane.Unbind();
		vbPlane->Unbind();
		ibPlane->Unbind();

		modelCylendir = glm::mat4(1.0f);
		modelPlatform = glm::mat4(1.0f);

		modelPlatform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.30f));

		sb = new ShadowBuffer();
		shaderBasic.Bind();
		shaderBasic.SetUniform1i("shadowMap", 1);
	}

	~Sandbox()
	{
		delete vb;
		delete ib;
		delete camera;
		delete sb;
	}

	void OnUpdate() override
	{
#pragma region "Shadow"
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
		glm::vec3 lightPos(10.0f, 10.0f, 10.0f);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		shaderDepthBasic.Bind();
		shaderDepthBasic.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, 1024, 1024);
		sb->BindFrameBuffer();
		RenderScene(shaderDepthBasic);
		sb->UnbindFrameBuffer();
#pragma endregion

		glViewport(0, 0, 1280, 720);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->UpdateProjMatrix();
		view = camera->GetViewMatrix();
		projection = camera->GetProjMatrix();
		ProcessInput();
		shaderBasic.Bind();
		shaderBasic.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
		shaderBasic.SetUniformMat4f("view", view);
		shaderBasic.SetUniformMat4f("projection", projection);
		shaderBasic.SetUniformVec3f("camPos", camera->GetEye());
		shaderBasic.SetUniformVec3f("lightPos", lightPos);
		sb->BindTexture();
		RenderScene(shaderBasic);
	}

	void ProcessInput()
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

	void RenderScene(Shader& shader)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader.SetUniformMat4f("model", modelCylendir);
		va.Bind();
		glDrawElements(GL_TRIANGLES, cylender.ShapeIndices().size(), GL_UNSIGNED_INT, 0);

		shader.SetUniformMat4f("model", modelPlatform);
		vaPlane.Bind();
		glDrawElements(GL_TRIANGLES, plane.ShapeIndices().size(), GL_UNSIGNED_INT, 0);
	}

public:
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 modelCylendir;
	glm::mat4 modelPlatform;

private:
#pragma region "Private Definitions"
    VertexArray va;
    VertexBuffer* vb;
    IndexBuffer* ib;
	ShadowBuffer* sb;

	VertexArray vaPlane;
	VertexBuffer* vbPlane;
	IndexBuffer* ibPlane;

	Shapes::Camera* camera;
	Shapes::Cylinder cube;
	Shapes::Cylinder cylender;
	Shapes::Plane plane;
	
	Shader shaderBasic;
	Shader shaderDepthBasic;

	glm::vec3 cameraPos = glm::vec3(3.0f, 3.0f, 3.0f);
	glm::vec3 cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
	float deltaTime = 0.0f;

	glm::vec2 lastMousePosRightClick = glm::vec2(0.0f, 0.0f);
	glm::vec2 currentMousePosClick = glm::vec2(0.0f, 0.0f);;
	bool firstMouseClick;
#pragma endregion
};

int main()
{
	Sandbox* s = new Sandbox();
	s->GetWindow().GetWidth();
	s->Run();
	s->OnUpdate();
	delete(s);
}