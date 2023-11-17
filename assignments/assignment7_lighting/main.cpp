#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>

#include <iostream>
#include <vector>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;
int MAX_lIGHTS = 3;

float prevTime;
ew::Vec3 bgColor = ew::Vec3(0.1f);

ew::Camera camera;
ew::CameraController cameraController;

struct Light {
	ew::Vec3 position;
	ew::Vec3 color; 
};

struct Material {
	float ambientK; 
	float diffuseK; 
	float specular; 
	float shininess;
};

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
	ew::Shader lightShader("assets/unlit.vert", "assets/unlit.frag");

	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg", GL_REPEAT, GL_LINEAR);

	//Create cube
	ew::Mesh cubeMesh(ew::createCube(1.0f));
	ew::Mesh planeMesh(ew::createPlane(5.0f, 5.0f, 10));
	ew::Mesh sphereMesh(ew::createSphere(0.5f, 64));
	ew::Mesh cylinderMesh(ew::createCylinder(0.5f, 1.0f, 32));

	ew::Mesh lightMesh(ew::createSphere(0.1f, 64));

	//Initialize transforms
	ew::Transform cubeTransform;
	ew::Transform planeTransform;
	ew::Transform sphereTransform;
	ew::Transform cylinderTransform;
	ew::Transform lightSphere;

	//Set object positions
	planeTransform.position = ew::Vec3(0, -1.0, 0);
	sphereTransform.position = ew::Vec3(-1.5f, 0.0f, 0.0f);
	cylinderTransform.position = ew::Vec3(1.5f, 0.0f, 0.0f);

	resetCamera(camera, cameraController);

	//Create light sources list each with defalt values
	std::vector<Light> lights;

	Light lights0;
	Light lights1;
	Light lights2;

	lights0.position = ew::Vec3(0.0f, 2.0f, 0.0f);
	lights0.color = ew::Vec3(0.0f, 1.0f, 1.0f);
	lights.push_back(lights0);

	lights1.position = ew::Vec3(1.0f, 2.0f, -1.0f);
	lights1.color = ew::Vec3(1.0f, 1.0f, 0.0f);
	lights.push_back(lights1);

	lights2.position = ew::Vec3(-1.5f, 0.0f, 1.5f);
	lights2.color = ew::Vec3(0.5f, 0.5f, 1.0f);
	lights.push_back(lights2);

	int activeLights = lights.size();

	Material material;

	material.ambientK = 0.2f;
	material.diffuseK = 0.9f;
	material.specular = 0.3f;
	material.shininess = 20.0f;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Calculate Delta Time
		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);

		glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		shader.setInt("_Texture", 0);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		shader.setInt("_NumLights", activeLights);

		shader.setFloat("material.ambientK", material.ambientK);
		shader.setFloat("material.diffuseK", material.diffuseK);
		shader.setFloat("material.specular", material.specular);
		shader.setFloat("material.shininess", material.shininess);

		shader.setInt("numLights", static_cast<int>(lights.size()));
		for (int i = 0; i < activeLights; ++i) {
			shader.setVec3("lights[" + std::to_string(i) + "].position", lights[i].position);
			shader.setVec3("lights[" + std::to_string(i) + "].color", lights[i].color);
		}

		// Draw objects and set materials
		shader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw();
		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw();
		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw();
		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw();

		// Render light sources
		for (int i = 0; i < activeLights; ++i) {
			lightShader.use();
			lightSphere.position = lights[i].position;
			lightShader.setMat4("_Model", lightSphere.getModelMatrix());
			lightShader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
			lightShader.setVec3("_Color", lights[i].color);
			lightMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}
			}

			ImGui::ColorEdit3("BG color", &bgColor.x);

			if (ImGui::CollapsingHeader("Material")) {
				ImGui::DragFloat("AmbientK", &material.ambientK, 0.001f);
				ImGui::DragFloat("DiffuseK", &material.diffuseK, 0.001f);
				ImGui::DragFloat("Shininess", &material.shininess, 0.1f);
				ImGui::DragFloat("Specular", &material.specular, 0.1f);
			}
			if (ImGui::CollapsingHeader("Lights")) {
				for (int i = 0; i < activeLights; ++i) {
					std::string lightLabel = "Light " + std::to_string(i);
					if (ImGui::CollapsingHeader(lightLabel.c_str())) {
						ImGui::DragFloat3("Position", &lights[i].position.x, 0.1f);
						ImGui::DragFloat3("Color", &lights[i].color.x, 0.1f);
					}
				}
				ImGui::SliderInt("Active Lights", &activeLights, 0, MAX_lIGHTS);
			}
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}