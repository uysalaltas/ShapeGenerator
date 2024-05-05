#include <imgui.h>

#include "ShapeGenerator.h"
#include "Teapot.h"

class Sandbox : public Teapot::Application
{
public:
	Sandbox(const Teapot::WindowProps& props = Teapot::WindowProps::WindowProps()) : 
		Teapot::Application(props)
		, cylender		(0.30f, glm::vec3(1.0f, 0.15f, 0.50f))
		, plane			(10, 10, 1.0f, glm::vec3(0.20f, 0.15f, 0.15f))
		, cube			(0.30f, glm::vec3(1.0f,  0.87f, 0.0f))
		, cubeLight		(0.10f, glm::vec3(1.0f,  1.0f , 1.0f))
		, pyramid		(0.30f, glm::vec3(0.20f, 0.71f, 0.29f), 2.0f, 4, 1.0f, 0.0f)
		, sphere		(0.30f, glm::vec3(0.20f, 0.25f, 1.0f), 30.0f, 30.0f)
		, shaderDepthBasic	("../vendor/Teapot/Teapot/shaders/BasicDepth.shader")
		, shaderBasic		("../vendor/Teapot/Teapot/shaders/PBR_NoTexture.shader")
	{
		camera = new Teapot::Camera(cameraPos, cameraCenter, cameraUp, (int*)this->GetWindow().GetWidthRef(), (int*)this->GetWindow().GetHeigthRef());

		cubeModel     = new Teapot::Model(cube.ShapePositions()     , cube.ShapeColors()     , cube.ShapeNormals()     , cube.ShapeIndices()     , "Cube");
		cubeLightModel= new Teapot::Model(cubeLight.ShapePositions(), cubeLight.ShapeColors(), cubeLight.ShapeNormals(), cubeLight.ShapeIndices(), "Light");
		planeModel    = new Teapot::Model(plane.ShapePositions()    , plane.ShapeColors()    , plane.ShapeNormals()    , plane.ShapeIndices()    , "Plane");
		pyramidModel  = new Teapot::Model(pyramid.ShapePositions()  , pyramid.ShapeColors()  , pyramid.ShapeNormals()  , pyramid.ShapeIndices()  , "Pyramid");
		cylinderModel = new Teapot::Model(cylender.ShapePositions() , cylender.ShapeColors() , cylender.ShapeNormals() , cylender.ShapeIndices() , "Cylender");
		sphereModel   = new Teapot::Model(sphere.ShapePositions()   , sphere.ShapeColors()   , sphere.ShapeNormals()   , sphere.ShapeIndices()   , "Sphere");

		cubeModel     ->Translate(glm::vec3( 1.0f,  0.0f,  0.0f ));
		planeModel    ->Translate(glm::vec3(-5.0f, -5.0f, -0.3f ));
		pyramidModel  ->Translate(glm::vec3( 2.0f,  0.0f,  0.0f ));
		cylinderModel ->Translate(glm::vec3(-1.0f,  0.0f,  0.0f ));

		models.push_back(cubeModel);
		models.push_back(cubeLightModel);
		models.push_back(planeModel);
		models.push_back(pyramidModel);
		models.push_back(cylinderModel);
		models.push_back(sphereModel);

		shadow = new Teapot::Shadow(shaderBasic, shaderDepthBasic);
	}

	~Sandbox()
	{
		delete camera;
		delete shadow;
		delete cubeModel;
		delete planeModel;
		delete cylinderModel;
		delete pyramidModel;
		delete sphereModel;
	}

	void OnUpdateAwake() override
	{
		shadow->RenderShadow(lightPos, models);
	}

	void OnUpdate() override
	{
		camera->UpdateProjMatrix();
		view = camera->GetViewMatrix();
		projection = camera->GetProjMatrix();
		camera->CalculateArcballCamera();
		camera->CalculatePanCamera();
		
		shaderBasic.Bind();
		shaderBasic.SetUniformMat4f("view", view);
		shaderBasic.SetUniformMat4f("projection", projection);
		shaderBasic.SetUniformMat4f("lightSpaceMatrix", shadow->GetLightSpaceMatrix());
		shaderBasic.SetUniformVec3f("camPos", camera->GetEye());
		shaderBasic.SetUniformVec3f("lightPos", lightPos);

		shaderBasic.SetUniform1f("ambientStrength", ambientStrength);
		shaderBasic.SetUniform1f("specularStrength", specularStrength);
		shaderBasic.SetUniform1f("shininess", shininess);
		
		shadow->BindShadow();
		RenderScene(shaderBasic);

		cubeLightModel->Translate(lightPos);
		
		// Ui Stuff
		//ImGui::ShowDemoWindow();
		ImGui::Begin("Transform", NULL, 0);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat3("Light Position", &lightPos[0], -10.0f, 10.0f);
		ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &shininess, 0.0f, 256.0f);

		ImGui::End();
	}

	void RenderScene(Teapot::Shader& shader)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cubeModel     ->Draw(shader);
		cubeLightModel->Draw(shader);
		planeModel    ->Draw(shader);
		pyramidModel  ->Draw(shader);
		cylinderModel ->Draw(shader);
		sphereModel   ->Draw(shader);
	}

public:
	glm::mat4 projection;
	glm::mat4 view;

private:
	std::vector<Teapot::Model*> models;

	Shapes::Cylinder cylender;
	Shapes::Plane    plane;
	Shapes::Cube     cube;
	Shapes::Cube     cubeLight;
	Shapes::Cylinder pyramid;
	Shapes::Sphere   sphere;
	
	Teapot::Camera* camera;
	Teapot::Shadow* shadow;

	Teapot::Model* cubeModel;
	Teapot::Model* cubeLightModel;
	Teapot::Model* planeModel;
	Teapot::Model* cylinderModel;
	Teapot::Model* pyramidModel;
	Teapot::Model* sphereModel;

	Teapot::Shader shaderDepthBasic;
	Teapot::Shader shaderBasic;

	glm::vec3 cameraPos    = glm::vec3(3.0f, 3.0f, 3.0f);
	glm::vec3 cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp     = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 lightPos     = glm::vec3(1.0f, 1.0f, 5.0f);

	float ambientStrength  = 0.40f;
	float specularStrength = 0.35f;
	float shininess	       = 50.0f;
};

int main()
{
	Teapot::WindowProps windowProps = { "Shape Demo", 1280, 720};
	Sandbox* s = new Sandbox(windowProps);
	s->GetWindow().GetWidth();
	s->Run();
	s->OnUpdate();
	delete(s);
}