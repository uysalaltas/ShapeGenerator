#include "ShapeGenerator.h"
#include "Teapot.h"
#include "ShapeGenerator/Cube.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ShadowMapping.h"

class Sandbox : public Teapot::Application
{
public:
	Sandbox()
		: cylender(0.30f, glm::vec3(1.0f, 0.15f, 0.50f))
		, plane(10, 10, 1.0f,glm::vec3(0.78f, 0.95f, 1.0f))
		, cube(0.30f, glm::vec3(1.0f, 0.87f, 0.0f))
		, pyramid(0.30f, glm::vec3(0.20f, 0.71f, 0.29f), 2.0f, 4, 1.0f, 0.0f)
		, shaderDepthBasic("src/BasicDepth.shader")
		, shaderDepthDebug("src/BasicDepthDebug.shader")
		, shaderBasic("src/Basic.shader")
	{
		camera = new Shapes::Camera(cameraPos, cameraCenter, cameraUp, (float)this->GetWindow().GetWidth(), (float)this->GetWindow().GetHeigth());

		vaCylendir.Bind();
		vbCylendir = new VertexBuffer(cylender.ShapeVertices());
		ibCylendir = new IndexBuffer(cylender.ShapeIndices());
		vaCylendir.AddBuffer(*vbCylendir, 0, 3, sizeof(Shapes::Vertex), (void*)0);
		vaCylendir.AddBuffer(*vbCylendir, 1, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::color));
		vaCylendir.AddBuffer(*vbCylendir, 2, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::normal));
		vaCylendir.Unbind();
		vbCylendir->Unbind();
		ibCylendir->Unbind();

		vaPlane.Bind();
		vbPlane = new VertexBuffer(plane.ShapeVertices());
		ibPlane = new IndexBuffer(plane.ShapeIndices());
		vaPlane.AddBuffer(*vbPlane, 0, 3, sizeof(Shapes::Vertex), (void*)0);
		vaPlane.AddBuffer(*vbPlane, 1, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::color));
		vaPlane.AddBuffer(*vbPlane, 2, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::normal));
		vaPlane.Unbind();
		vbPlane->Unbind();
		ibPlane->Unbind();

		vaCube.Bind();
		vbCube = new VertexBuffer(cube.ShapeVertices());
		ibCube = new IndexBuffer(cube.ShapeIndices());
		vaCube.AddBuffer(*vbCube, 0, 3, sizeof(Shapes::Vertex), (void*)0);
		vaCube.AddBuffer(*vbCube, 1, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::color));
		vaCube.AddBuffer(*vbCube, 2, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::normal));
		vaCube.Unbind();
		vbCube->Unbind();
		ibCube->Unbind();

		vaPyramid.Bind();
		vbPyramid = new VertexBuffer(pyramid.ShapeVertices());
		ibPyramid = new IndexBuffer(pyramid.ShapeIndices());
		vaPyramid.AddBuffer(*vbPyramid, 0, 3, sizeof(Shapes::Vertex), (void*)0);
		vaPyramid.AddBuffer(*vbPyramid, 1, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::color));
		vaPyramid.AddBuffer(*vbPyramid, 2, 3, sizeof(Shapes::Vertex), (void*)offsetof(Shapes::Vertex, Shapes::Vertex::normal));
		vaPyramid.Unbind();
		vbPyramid->Unbind();
		ibPyramid->Unbind();

		modelCylendir = glm::mat4(1.0f);
		modelPlatform = glm::mat4(1.0f);
		modelCube = glm::mat4(1.0f);
		modelPyramid = glm::mat4(1.0f);

		modelPlatform = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -5.0f, -0.3f));
		modelCube = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelPyramid = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));

		shadow = new ShadowMapping();
		// shader configuration
		// --------------------
		shaderBasic.Bind();
		shaderBasic.SetUniform1i("shadowMap", 1);
		shaderDepthDebug.Bind();
		shaderDepthDebug.SetUniform1i("depthMap", 0);
	}

	~Sandbox()
	{
		delete vbCylendir;
		delete ibCylendir;

		delete vbPlane;
		delete ibPlane;

		delete vbCube;
		delete ibCube;

		delete vbPyramid;
		delete ibPyramid;

		delete camera;
		delete shadow;
	}

	void OnUpdate() override
	{
		glEnable(GL_DEPTH_TEST);

		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
		glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		shadow->RenderShadow(shaderDepthBasic, lightSpaceMatrix);
		RenderScene(shaderDepthBasic);
		shadow->UnbindFrameBuffer();

		glViewport(0, 0, 1280, 720);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->UpdateProjMatrix();
		view = camera->GetViewMatrix();
		projection = camera->GetProjMatrix();
		ProcessInput();
		shaderBasic.Bind();
		shaderBasic.SetUniformMat4f("view", view);
		shaderBasic.SetUniformMat4f("projection", projection);
		shaderBasic.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
		shaderBasic.SetUniformVec3f("camPos", camera->GetEye());
		shaderBasic.SetUniformVec3f("lightPos", lightPos);
		shadow->BindTexture();
		RenderScene(shaderBasic);

		//Debug Shadow
		//shadow->DebugShadow(shaderDepthDebug);
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
		vaCylendir.Bind();
		glDrawElements(GL_TRIANGLES, cylender.ShapeIndices().size(), GL_UNSIGNED_INT, 0);

		shader.SetUniformMat4f("model", modelPlatform);
		vaPlane.Bind();
		glDrawElements(GL_TRIANGLES, plane.ShapeIndices().size(), GL_UNSIGNED_INT, 0);

		shader.SetUniformMat4f("model", modelCube);
		vaCube.Bind();
		glDrawElements(GL_TRIANGLES, cube.ShapeIndices().size(), GL_UNSIGNED_INT, 0);

		shader.SetUniformMat4f("model", modelPyramid);
		vaPyramid.Bind();
		glDrawElements(GL_TRIANGLES, cylender.ShapeIndices().size(), GL_UNSIGNED_INT, 0);
	}

public:
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 modelCylendir;
	glm::mat4 modelPlatform;
	glm::mat4 modelCube;
	glm::mat4 modelPyramid;

private:
	VertexArray vaCylendir;
	VertexBuffer* vbCylendir;
	IndexBuffer* ibCylendir;

	VertexArray vaPlane;
	VertexBuffer* vbPlane;
	IndexBuffer* ibPlane;

	VertexArray vaCube;
	VertexBuffer* vbCube;
	IndexBuffer* ibCube;

	VertexArray vaPyramid;
	VertexBuffer* vbPyramid;
	IndexBuffer* ibPyramid;

	Shapes::Camera* camera;
	Shapes::Cylinder cylender;
	Shapes::Plane plane;
	Shapes::Cube cube;
	Shapes::Cylinder pyramid;
	
	Shader shaderDepthBasic;
	Shader shaderDepthDebug;
	Shader shaderBasic;

	glm::vec3 cameraPos = glm::vec3(3.0f, 3.0f, 3.0f);
	glm::vec3 cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
	float deltaTime = 0.0f;

	glm::vec2 lastMousePosRightClick = glm::vec2(0.0f, 0.0f);
	glm::vec2 currentMousePosClick = glm::vec2(0.0f, 0.0f);;
	bool firstMouseClick;

	const glm::vec3 lightPos = glm::vec3(5.0f, 1.0f, 5.0f);

	ShadowMapping* shadow;
};

int main()
{
	Sandbox* s = new Sandbox();
	s->GetWindow().GetWidth();
	s->Run();
	s->OnUpdate();
	delete(s);
}