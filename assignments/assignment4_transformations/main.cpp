#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/ewMath/vec3.h>
#include <ew/procGen.h>
#include <ew/mesh.h>
#include <as/transformations.h>
#include "as/camera.h"


void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;
const int NUM_CUBES = 4;

as::Camera camera;

int main() {
    printf("Initializing...");
    if (!glfwInit()) {
        printf("GLFW failed to init!");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures", NULL, NULL);
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
    as::Transform transform;

    ew::Mesh cubeMesh(ew::createCube(0.5f));

    std::vector<as::Transform> transforms(NUM_CUBES);

    transforms[0].position = ew::Vec3(-0.5f, -0.5f, 0.0f);
    transforms[1].position = ew::Vec3(0.5f, -0.5f, 0.0f);
    transforms[2].position = ew::Vec3(-0.5f, 0.5f, 0.0f);
    transforms[3].position = ew::Vec3(0.5f, 0.5f, 0.0f);

    // Initialize Camera
    camera.position = ew::Vec3(0.0f, 0.0f, 5.0f);
    camera.target = ew::Vec3(0.0f, 0.0f, 0.0f);
    camera.fov = 60.0f;
    camera.orthoHeight = 6.0f;
    camera.nearPlane = 0.1f;
    camera.farPlane = 100.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("_View", camera.getViewMatrix());
        shader.setMat4("_Projection", camera.getProjectionMatrix(SCREEN_WIDTH / static_cast<float>(SCREEN_HEIGHT)));

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Transform");

        for (int i = 0; i < NUM_CUBES; i++) {
            shader.setMat4("_Model", transforms[i].getModelMatrix());
            cubeMesh.draw();

            ImGui::PushID(i);
            ImGui::Text("Cube %d", i);
            ImGui::DragFloat3("Position", &transforms[i].position.x, 0.05f);
            ImGui::DragFloat3("Rotation", &transforms[i].rotation.x, 1.0f);
            ImGui::DragFloat3("Scale", &transforms[i].scale.x, 0.05f);
            ImGui::Separator();
            ImGui::PopID();
        }

        // Camera Controls
        ImGui::Begin("Camera Controls");
        ImGui::DragFloat3("Camera Position", &camera.position.x, 0.05f);
        ImGui::DragFloat3("Camera Target", &camera.target.x, 0.05f);
        ImGui::SliderFloat("FOV", &camera.fov, 1.0f, 180.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    camera.updateProjection(width / static_cast<float>(height));
}
