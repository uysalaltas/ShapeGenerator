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
	{
		camera = new Teapot::Camera(cameraPos, cameraCenter, cameraUp, this->GetWindow().GetWidthRef(), this->GetWindow().GetHeigthRef());
		cubeModel      = new Teapot::Model(cube.ShapePositions()     , cube.ShapeColors()     , cube.ShapeNormals()     , cube.ShapeIndices()     , "Cube");
		cubeLightModel = new Teapot::Model(cubeLight.ShapePositions(), cubeLight.ShapeColors(), cubeLight.ShapeNormals(), cubeLight.ShapeIndices(), "Light");
		planeModel     = new Teapot::Model(plane.ShapePositions()    , plane.ShapeColors()    , plane.ShapeNormals()    , plane.ShapeIndices()    , "Plane");
		pyramidModel   = new Teapot::Model(pyramid.ShapePositions()  , pyramid.ShapeColors()  , pyramid.ShapeNormals()  , pyramid.ShapeIndices()  , "Pyramid");
		cylinderModel  = new Teapot::Model(cylender.ShapePositions() , cylender.ShapeColors() , cylender.ShapeNormals() , cylender.ShapeIndices() , "Cylender");
		sphereModel    = new Teapot::Model(sphere.ShapePositions()   , sphere.ShapeColors()   , sphere.ShapeNormals()   , sphere.ShapeIndices()   , "Sphere");

		cubeModel     ->Translate(glm::vec3( 1.0f,  0.0f,  0.0f ));
		planeModel    ->Translate(glm::vec3(-5.0f, -5.0f, -0.3f ));
		pyramidModel  ->Translate(glm::vec3( 2.0f,  0.0f,  0.0f ));
		cylinderModel ->Translate(glm::vec3(-1.0f,  0.0f,  0.0f ));

		Teapot::ShaderManager::GetInstance()->SetShaderValues(*camera);
	}

	~Sandbox()
	{
		delete camera;
		//delete shadow;
		delete cubeModel;
		delete planeModel;
		delete cylinderModel;
		delete pyramidModel;
		delete sphereModel;
	}

	void OnUpdateAwake() override
	{
		//shadow->RenderShadow(lightPos, models);
	}

	void OnUpdate() override
	{
		camera->UpdateProjMatrix();
		camera->CalculateArcballCamera();
		camera->CalculatePanCamera();
		camera->ZoomCamera();

		shaderManager->RunShader();
		RenderScene();

		cubeLightModel->Translate(shaderManager->GetLightPos());
		
		// Ui Stuff
		//ImGui::ShowDemoWindow();
		ImGui::Begin("Transform", NULL, 0);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat3("Light Position", &shaderManager->GetLightPos()[0], -10.0f, 10.0f);
		ImGui::SliderFloat("Ambient Strength", &ambientStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular Strength", &specularStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &shininess, 0.0f, 256.0f);

		ImGui::End();
	}

	void RenderScene()
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cubeModel     ->Draw();
		cubeLightModel->Draw();
		planeModel    ->Draw();
		pyramidModel  ->Draw();
		cylinderModel ->Draw();
		sphereModel   ->Draw();
	}

private:
	Shapes::Cylinder cylender;
	Shapes::Plane    plane;
	Shapes::Cube     cube;
	Shapes::Cube     cubeLight;
	Shapes::Cylinder pyramid;
	Shapes::Sphere   sphere;
	
	Teapot::Camera* camera;
	Teapot::Model* cubeModel;
	Teapot::Model* cubeLightModel;
	Teapot::Model* planeModel;
	Teapot::Model* cylinderModel;
	Teapot::Model* pyramidModel;
	Teapot::Model* sphereModel;

	glm::vec3 cameraPos    { 3.0f, 3.0f, 3.0f };
	glm::vec3 cameraCenter { 0.0f, 0.0f, 0.0f };
	glm::vec3 cameraUp     { 0.0f, 0.0f, 1.0f };

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