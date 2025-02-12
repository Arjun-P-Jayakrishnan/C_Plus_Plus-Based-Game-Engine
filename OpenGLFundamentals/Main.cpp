#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Model.h"





unsigned int width = 800;
unsigned int height = 800;

int main() {

	//initialize
	glfwInit();
	//Specify the version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//GLFW_OPENGL_PROFILE -CORE USES MODERN PACKAGES 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	GLFWwindow* window = glfwCreateWindow(width, height, "Opengl Programming", NULL,NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Introduce window into current context
	glfwMakeContextCurrent(window);

	//using glad library to change colors
	gladLoadGL();

	//Specify the viewport of OpenGl in window
	//In this case it goes from 0,0 to 800,800
	glViewport(0,0,width,height);
	


	//Generates shader object using default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	


	glm::vec4 lightColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f,0.5f,0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z,lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID,"lightPos"),lightPos.x,lightPos.y,lightPos.z);

	

	

	//specify the background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);




	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));




	std::string modelPath = "D:/sword/scene.gltf";

	Model model(modelPath.c_str());
	
	//Run until close button is pressed
	while (!glfwWindowShouldClose(window)) {
		//Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean teh back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
		//Handles Camera Inputs
		camera.Inputs(window);
		//Update Matrix
		camera.updateMatrix(45.0f,0.1f,100.0f);
		
		
		model.Draw(shaderProgram, camera);


		//Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		//Take care of all glfw events
		glfwPollEvents();
	}

	
	shaderProgram.Delete();

	

	//Delete the window before closing the program
	glfwDestroyWindow(window);
	//Close the program
	glfwTerminate();


	return 0;
}