#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "model.hpp"
#include "camera.hpp"

void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow *, double, double);
void scroll_callback(GLFWwindow *, double, double);
void processInput(GLFWwindow *);

bool firstMouse = true;
const unsigned int W = 1080;
const unsigned int H = 720;
float lastX = W / 2.f;
float lastY = H / 2.f;
float deltaTime = 0.f;
float lastFrame = 0.f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main(int argc, char *argv[]) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(W, H, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cout<<"Failed to create GLFW window"<<"\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout<<"Failed to initialize GLAD\n";
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	Shader shader("../config/model_loading.vs","../config/model_loading.fs");
	Shader shader_sample("../config/forward.vs","../config/forward.fs");
	GLModel gl_model("../Material/bus_setia_negara_texturizer.blend");
	//GLModel gl_model("../14-lowpolyfiatuno/LowPolyFiatUNO.obj");
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		//glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClearColor(0.95f, 0.95f, 0.95f, 0.5f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
		model = glm::rotate(model, glm::degrees(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);	
		shader.setMat4("model",model);
		shader.setMat4("view",view);
		shader.setMat4("projection",projection);
		//gl_model.Draw(shader);

		shader_sample.use();
		float fc_len = W / H;
		float skew = 0.f;
		float K_data[16] = {
			fc_len, skew, 0.5f, 0.f,
			 0.f, fc_len, 0.5f, 0.f,
			 0.f, 0.f, 0.f, 0.f,
			 0.f, 0.f, 0.f, 0.f
		};
		/*
		float K_data[16] = {
			 fc_len, skew, 0.5f, 0.f,
			 0.f, fc_len, 0.5f, 0.f,
			 0.f, 0.f, 0.f, 0.f,
			 0.f, 0.f, 0.f, 0.f
		};
		*/
		
		glm::mat4 K;
		memcpy(glm::value_ptr(K), K_data, sizeof(K_data));
		glm::mat4 pose = glm::mat4(1.0f);	
		pose = glm::translate(pose, glm::vec3(0.f,10.f,0.f));
		pose = glm::inverse(pose);
		shader_sample.setMat4("K",K);
		shader_sample.setMat4("pose",pose);
		shader_sample.setMat4("view",view);
		shader_sample.setMat4("projection",projection);

		float vertices[] = {
			10.f, 3.f, 0.f,
			10.f, 10.f, 0.f,
			3.f, 10.f, 0.f,
			3.f, 3.f, 0.f
			/*
			0.5f,  0.5f, 0.5f,  // top right
			 0.5f, -0.5f, 0.5f,  // bottom right
			-0.5f, -0.5f, 0.5f,  // bottom left
			-0.5f,  0.5f, 0.5f   // top left 
			*/
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		unsigned int VAO1;
		glGenVertexArrays(1, &VAO1);
		glBindVertexArray(VAO1);
		unsigned int VBO1;
		glGenBuffers(1, &VBO1);
		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		unsigned int EBO1;
		glGenBuffers(1, &EBO1);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(3); 
	
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		//gl_model.Draw(shader_sample.ID);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos-lastX;
	float yoffset = lastY-ypos;
	lastX=xpos;
	lastY=ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}



