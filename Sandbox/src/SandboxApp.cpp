#include "ShapeGenerator.h"
#include "Teapot.h"
#include <Renderer/FrameBuffer.h>

class Sandbox : public Teapot::Application
{
public:
	Sandbox()
		: cylender(0.30f, glm::vec3(1.0f, 0.15f, 0.50f))
		, plane(10, 10, 1.0f, glm::vec3(0.78f, 0.95f, 1.0f))
		, cube(0.30f, glm::vec3(1.0f, 0.87f, 0.0f))
		, pyramid(0.30f, glm::vec3(0.20f, 0.71f, 0.29f), 2.0f, 4, 1.0f, 0.0f)
		, shaderDepthBasic("src/BasicDepth.shader")
		, shaderDepthDebug("src/BasicDepthDebug.shader")
		, shaderBasic("src/Basic.shader")
	{
		camera = new Teapot::Camera(cameraPos, cameraCenter, cameraUp, (int*)this->GetWindow().GetWidthRef(), (int*)this->GetWindow().GetHeigthRef());

		cubeModel = new Teapot::Model(cube.ShapePositions(), cube.ShapeColors(), cube.ShapeNormals(), cube.ShapeIndices(), "Cube");
		planeModel = new Teapot::Model(plane.ShapePositions(), plane.ShapeColors(), plane.ShapeNormals(), plane.ShapeIndices(), "Plane");
		pyramidModel = new Teapot::Model(pyramid.ShapePositions(), pyramid.ShapeColors(), pyramid.ShapeNormals(), pyramid.ShapeIndices(), "Pyramid");
		cylinderModel = new Teapot::Model(cylender.ShapePositions(), cylender.ShapeColors(), cylender.ShapeNormals(), cylender.ShapeIndices(), "Cylender");

		planeModel->Translate(glm::vec3(-5.0f, -5.0f, -0.3f));
		cubeModel->Translate(glm::vec3(1.0f, 0.0f, 0.0f));
		pyramidModel->Translate(glm::vec3(2.0f, 0.0f, 0.0f));

		shadow = new Teapot::ShadowMapping();
		// shader configuration
		// --------------------
		shaderBasic.Bind();	
		shaderBasic.SetUniform1i("shadowMap", 1);
		shaderDepthDebug.Bind();
		shaderDepthDebug.SetUniform1i("depthMap", 0);
	}

	~Sandbox()
	{
		delete camera;
		delete shadow;
		delete cubeModel;
		delete planeModel;
		delete cylinderModel;
		delete pyramidModel;
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
		camera->CalculateArcballCamera();
		
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

	void RenderScene(Teapot::Shader& shader)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cubeModel->Draw(shader);
		planeModel->Draw(shader);
		pyramidModel->Draw(shader);
		cylinderModel->Draw(shader);
	}

public:
	glm::mat4 projection;
	glm::mat4 view;

private:
	Shapes::Cylinder cylender;
	Shapes::Plane plane;
	Shapes::Cube cube;
	Shapes::Cylinder pyramid;
	
	Teapot::Camera* camera;

	Teapot::Model* cubeModel;
	Teapot::Model* planeModel;
	Teapot::Model* cylinderModel;
	Teapot::Model* pyramidModel;

	Teapot::Shader shaderDepthBasic;
	Teapot::Shader shaderDepthDebug;
	Teapot::Shader shaderBasic;

	glm::vec3 cameraPos = glm::vec3(3.0f, 3.0f, 3.0f);
	glm::vec3 cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);

	const glm::vec3 lightPos = glm::vec3(5.0f, 1.0f, 5.0f);

	Teapot::ShadowMapping* shadow;
};

int main()
{
	Sandbox* s = new Sandbox();
	s->GetWindow().GetWidth();
	s->Run();
	s->OnUpdate();
	delete(s);
}