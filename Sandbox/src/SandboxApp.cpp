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
		: cylender(0.30f, glm::vec3(1.0f, 1.0f, 1.0f))
		, plane(3.0f, glm::vec3(0.78f, 0.95f, 1.0f))
		, shaderDepthBasic("src/BasicDepth.shader")
		, shaderDepthDebug("src/BasicDepthDebug.shader")
		, shaderBasic("src/Basic.shader")
	{
		camera = new Shapes::Camera(cameraPos, cameraCenter, cameraUp, (float)this->GetWindow().GetWidth(), (float)this->GetWindow().GetHeigth());

#pragma region "Object"
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
		modelPlatform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.3f));
#pragma endregion

		glGenTextures(1, &depthMapTexture);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenFramebuffers(1, &depthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// shader configuration
		// --------------------
		shaderBasic.Bind();
		shaderBasic.SetUniform1i("shadowMap", 1);
		shaderDepthDebug.Bind();
		shaderDepthDebug.SetUniform1i("depthMap", 0);
	}

	~Sandbox()
	{
		delete vb;
		delete ib;
		delete camera;
	}

	void OnUpdate() override
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float near_plane = 1.0f, far_plane = 20.0f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(
			lightPos,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		shaderDepthBasic.Bind();
		shaderDepthBasic.SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		RenderScene(shaderDepthBasic);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		RenderScene(shaderBasic);

		shaderDepthDebug.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		//RenderQuad();
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
		shader.SetUniformMat4f("model", modelCylendir);
		va.Bind();
		glDrawElements(GL_TRIANGLES, cylender.ShapeIndices().size(), GL_UNSIGNED_INT, 0);

		shader.SetUniformMat4f("model", modelPlatform);
		vaPlane.Bind();
		glDrawElements(GL_TRIANGLES, plane.ShapeIndices().size(), GL_UNSIGNED_INT, 0);
	}

	void RenderQuad()
	{
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
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

	VertexArray vaPlane;
	VertexBuffer* vbPlane;
	IndexBuffer* ibPlane;

	Shapes::Camera* camera;
	Shapes::Cylinder cylender;
	Shapes::Plane plane;
	
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

	glm::vec3 lightPos = glm::vec3((3.0f, 3.0f, 3.0f));
#pragma endregion

	GLuint depthMapFBO;
	GLuint depthMapTexture;
	GLuint renderbuffer;


	unsigned int quadVAO = 0;
	unsigned int quadVBO;
};

int main()
{
	Sandbox* s = new Sandbox();
	s->GetWindow().GetWidth();
	s->Run();
	s->OnUpdate();
	delete(s);
}