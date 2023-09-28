#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <as/shader.h>


//unsigned int createShader(GLenum shaderType, const char* sourceCode);
//unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
unsigned int createVAO(float* vertexData, int numVertices, unsigned int* indicesData, int numIndices); 
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[12] = {
	// x    y    z
	0.0f, -1.0f, 0.0f, // Bottom left corner
	1.0f, -1.0f, 0.0f, // Bottom right corner
	1.0f,  0.0f, 0.0f, // Top right corner
	0.0f,  0.0f, 0.0f  // Top left corner
};


unsigned int indices[6] = {
	0, 1, 2, // Triangle 1 (Bottom left, Bottom right, Top right)
	2, 3, 0  // Triangle 2 (Top right, Top left, Bottom left)
};




float triangleColor[3] = { 1.0f, 0.5f, 0.0f };
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
	



	unsigned int vao = createVAO(vertices, 6, indices, 6);

 

	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		
		
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		

		shader.setVec3("_Color", triangleColor[0], triangleColor[1], triangleColor[2]); 
		shader.setFloat("_Brightness", triangleBrightness); 


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{

			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Color", triangleColor);
			ImGui::SliderFloat("Brightness", &triangleBrightness, 0.0f, 1.0f);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(float* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao;
	

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, vertexData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);
	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

