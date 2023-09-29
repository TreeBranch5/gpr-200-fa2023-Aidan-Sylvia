#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <as/shader.h>




const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float sunRadius = 0.3f;
float sunSpeed = 0.5f;

struct Vertex {
	float x, y, z;
	float u, v;
};
Vertex screenBox[4] = {
	//x    y    z    u    v
   { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f}, //Bottom left  
   {  1.0f, -1.0f, 0.0f, 1.0f, 0.0f}, //Bottom right 
   {  1.0f,  1.0f, 0.0f, 1.0f, 1.0f}, //Top right    
   { -1.0f,  1.0f, 0.0f, 0.0f, 1.0f}  //Top left     
};
unsigned int indices[6] = {
	0, 1, 2, //First triangle
	0, 2, 3  //Second triangle
};




unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

float skyColor[4] = { 1.0, 0.5, 0.0, 1.0 }; // Orange
float sunColor[4] = { 1.0, 1.0, 0.0, 1.0 }; // Yellow

float triangleBrightness = 1.0f;
bool showImGUIDemoWindow = true;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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

	as::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();

	unsigned int vao = createVAO(screenBox, 4, indices, 6);
	glBindVertexArray(vao);

	

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		shader.setVec2("iResolution", (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
		shader.setVec4("skyColor", skyColor[0], skyColor[1], skyColor[2], skyColor[3]);
		shader.setFloat("_Brightness", triangleBrightness);
		shader.setFloat("iTime", (float)glfwGetTime());
		shader.setFloat("radius", sunRadius);
		shader.setVec4("sunColor", sunColor[0], sunColor[1], sunColor[2], sunColor[3]);
		shader.setFloat("sunSpeed", sunSpeed);
	

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Sky Color", skyColor);
			ImGui::ColorEdit3("Sun Color", sunColor);
			ImGui::SliderFloat("Sun Radius", &sunRadius, 0.0f, 1.0f);
			ImGui::SliderFloat("Sun Speed", &sunSpeed, 0.0f, 5.0f);

			//ImGui::SliderFloat("Brightness", &triangleBrightness, 0.0f, 1.0f);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}
		

			// ... rest of your rendering code

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao, vbo, ebo;

	// Generate and bind VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generate and bind VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);

	// Define vertex attributes
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	// UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, u));
	glEnableVertexAttribArray(1);

	// Generate and bind EBO
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	return vao;
}


void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}