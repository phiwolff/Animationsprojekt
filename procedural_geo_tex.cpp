#include "common.hpp"
#include "shader.hpp"
#include "buffer.hpp"
#include <iostream>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/gtx/transform.hpp>
#include "camera.hpp"
#include "mesh.hpp"
using namespace std;
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const float FOV = 45.f;
const float NEAR_VALUE = 0.1f;
const float FAR_VALUE = 100.f;
const float SUN_EARTH_DISTANCE = 5.f;
const float EARTH_MOON_DISTANCE = 2.f;

glm::mat4 proj_matrix;

// called whenever the window gets resized
void resizeCallback(GLFWwindow* window, int width, int height);

// loads a shader source file, tries to compile the shader
// and checks for compilation errors
unsigned int compileShaderHelper(const char* filename, unsigned int type);

// checks linked program for linker errors
bool checkProgram(unsigned int program);

int main(int, char* argv[]) {
	GLFWwindow* window = initOpenGL(WINDOW_WIDTH, WINDOW_HEIGHT, argv[0]);
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	
	// Tex Anfang
	unsigned int vertexShader = compileShaderHelper("tex_proc.vert", GL_VERTEX_SHADER);
	unsigned int fragmentShader = compileShaderHelper("tex_proc.frag", GL_FRAGMENT_SHADER);
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	if (!checkProgram(shaderProgram)) {
		std::terminate();
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	float vertices[120];
	for (int i = 0; i < 10; i++) {
		vertices[i * 12] = -0.5 + 0.1 * i;
		vertices[i * 12 + 1] = -0.5;
		vertices[i * 12 + 2] = 0;
		vertices[i * 12 + 3] = -0.5 + 0.1 *i + 0.1;
		vertices[i * 12 + 4] = -0.5;
		vertices[i * 12 + 5] = 0;
		vertices[i * 12 + 6] = -0.5 + 0.1 *i + 0.1;
		vertices[i * 12 + 7] = 0.5;
		vertices[i * 12 + 8] = 0;
		vertices[i * 12 + 9] = -0.5 + 0.1 * i;
		vertices[i * 12 + 10] = 0.5;
		vertices[i * 12 + 11] = 0;
	}
	unsigned int indices[60];
	for (int i = 0; i < 10; i++) {
		indices[i * 6] = i * 4;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 3;
		indices[i * 6 + 5] = i * 4;
	}

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO = makeBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices);;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	unsigned int IBO = makeBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(indices), indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// Tex Ende
	
	
	//GEO Anfang
	camera cam(window);
	geometry sun = loadMesh("sphere.obj", false, glm::vec4(1.f, 0.6f, 0.f, 1.f));
	unsigned int vertexShaderGEO = compileShader("proc_geo_tex.vert", GL_VERTEX_SHADER);
	unsigned int fragmentShaderGEO = compileShader("proc_geo_tex.frag", GL_FRAGMENT_SHADER);
	unsigned int shaderProgramGEO;
	shaderProgramGEO = glCreateProgram();
	glAttachShader(shaderProgramGEO, vertexShaderGEO);
	glAttachShader(shaderProgramGEO, fragmentShaderGEO);
	glLinkProgram(shaderProgramGEO);
	if (!checkProgram(shaderProgramGEO)) {
		std::terminate();
	}
	glDeleteShader(fragmentShaderGEO);
	glDeleteShader(vertexShaderGEO);
	// hole Adressen der uniform variablen
	int model_mat_loc = glGetUniformLocation(shaderProgramGEO, "model_mat");
	int view_mat_loc = glGetUniformLocation(shaderProgramGEO, "view_mat");
	int proj_mat_loc = glGetUniformLocation(shaderProgramGEO, "proj_mat");
	proj_matrix = glm::perspective(FOV, 1.f, NEAR_VALUE, FAR_VALUE);
	int light_dir_loc = glGetUniformLocation(shaderProgramGEO, "light_dir");
	glm::vec3 light_dir = glm::normalize(glm::vec3(1.0, 1.0, 1.0));
	glUniform3fv(light_dir_loc, 1, &light_dir[2]);
	//glEnable(GL_DEPTH_TEST);
	float sunRise = 0.f;
	bool flipColor = true;
	int counter = 0;
	int loopCounter = 0;
	unsigned int VAO_GEO;
	//GEO Ende
	
	int switchCounter = 0;

	while (glfwWindowShouldClose(window) == false) {
		
		switchCounter++;

		if (switchCounter < 100) {
			// Tex Anfang
			glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 160, GL_UNSIGNED_INT, (void*)0);
			// Tex Ende
		}

		std::cout << switchCounter << "\n";

		
		if (switchCounter >= 100) {
			// GEO Anfang
			sunRise = sunRise + 0.001;
			glm::vec3 light_dir = glm::vec3(sunRise, sunRise, sunRise);
			if (loopCounter == 91) {
				loopCounter = 0;
			}
			if (counter == 50) {
				if (flipColor == true) {
					glClearColor(0.1f, 0.7f, 0.25f, 1.0f);
					flipColor = false;
					counter = 0;
				}
				else {
					glClearColor(0.5f, 0.3f, 0.6f, 1.0f);
					flipColor = true;
					counter = 0;
				}
			}
			counter++;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(shaderProgramGEO);
			glm::mat4 view_matrix = cam.view_matrix();
			glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, &view_matrix[0][0]);
			glUniformMatrix4fv(proj_mat_loc, 1, GL_FALSE, &proj_matrix[0][0]);
			glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, &sun.transform[0][0]);
			unsigned int num_segments = 30;
			unsigned int num_vertices = num_segments + 1;
			float radius = 0.5f;
			float* verticesGEO = new float[3 * num_vertices];
			unsigned int* indicesGEO = new unsigned int[3 * num_segments];
			// dieser Punkt erzeugt die Spitze des Kegels, da z = 1
			verticesGEO[0] = 0.f;
			verticesGEO[1] = 0.f;
			verticesGEO[2] = 1.f;
			for (unsigned int i = 0; i < num_segments; ++i) {
				float angle = (2.0 * M_PI * i) / num_segments;
				verticesGEO[3 + i * 3 + 0] = radius * cos(angle); // X
				verticesGEO[3 + i * 3 + 1] = radius * sin(angle); // Y
				verticesGEO[3 + i * 3 + 2] = 0.f;               // Z
				unsigned int current_index = i + 1;
				unsigned int next_index = current_index + 1;
				if (next_index > num_segments) {
					next_index = 1;
				}
				indicesGEO[i * 3 + 0] = 0;
				indicesGEO[i * 3 + 1] = current_index;
				indicesGEO[i * 3 + 2] = next_index;
			}
			unsigned int VAO_GEO;
			glGenVertexArrays(1, &VAO_GEO);
			glBindVertexArray(VAO_GEO);
			unsigned int VBO_GEO = makeBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, 3 * num_vertices * sizeof(float), verticesGEO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO_GEO);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			unsigned int IBO_GEO = makeBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, 3 * num_segments * sizeof(unsigned int), indicesGEO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_GEO);
			glUseProgram(shaderProgramGEO);
			glBindVertexArray(VAO_GEO);
			glDrawElements(GL_TRIANGLES, loopCounter, GL_UNSIGNED_INT, (void*)0);
			loopCounter++;
			//Geo Ende	
		}

		if (switchCounter == 300) {
			switchCounter = 0;
		}
		
		

		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}


	glfwTerminate();
}

void resizeCallback(GLFWwindow*, int width, int height)
{
	// set new width and height as viewport size
	glViewport(0, 0, width, height);
	proj_matrix = glm::perspective(FOV, static_cast<float>(width) / height, NEAR_VALUE, FAR_VALUE);
}

unsigned int
compileShaderHelper(const char* filename, unsigned int type) {
	const char* shaderSource = loadShaderFile(filename);

	// create shader object
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, NULL);
	// try to compile
	glCompileShader(shader);
	// source code is no longer needed
	delete[] shaderSource;

	// check if compilation succeeded
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "Shader compilation failed\n" << infoLog << std::endl;
		return 0;
	}

	return shader;
}

bool
checkProgram(unsigned int program) {
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "Linking program failed\n" << infoLog << std::endl;
		return false;
	}

	return true;
}
