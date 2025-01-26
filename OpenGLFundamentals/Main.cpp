#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

int main() {

	//initialize
	glfwInit();
	//Specify the version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//GLFW_OPENGL_PROFILE -CORE USES MODERN PACKAGES 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Opengl Programming", NULL,NULL);

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
	glViewport(0,0,800,800);

	//specify the background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	
	//Run until close button is pressed
	while (!glfwWindowShouldClose(window)) {
		//Take care of all glfw events
		glfwPollEvents();
	}

	//Delete the window before closing the program
	glfwDestroyWindow(window);
	//Close the program
	glfwTerminate();


	return 0;
}