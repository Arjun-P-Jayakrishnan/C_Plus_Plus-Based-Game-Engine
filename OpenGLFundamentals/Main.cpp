#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "shaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "Texture.h"



GLfloat vertices[] = {
	//COORDINATES                   // COLORS		       //TEXTURE
	-0.5f,		-0.5f,		0.0f,	1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
	-0.5f,		 0.5f,		0.0f,	0.0f,	1.0f,	0.0f,	0.0f,	1.0f,
	 0.5f,		 0.5f,		0.0f,	0.0f,	0.0f,	1.0f,	1.0f,	1.0f,
	 0.5f,		-0.5f,		0.0f,	1.0f,	1.0f,	1.0f,	1.0f,	0.0f
};

GLuint indices[] = {

		0,2,1,//Lower left triangle
		0,3,2,//Lower right triangle

};

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

	Shader shaderProgram("default.vert", "default.frag");
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0,3,GL_FLOAT,8*sizeof(float),(void*)0);
	VAO1.LinkAttrib(VBO1, 1,3,GL_FLOAT,8*sizeof(float),(void*)(3*sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2,2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID,"scale");

	Texture manTexture("man.png",GL_TEXTURE_2D,GL_TEXTURE0,GL_RGBA,GL_UNSIGNED_BYTE);
	manTexture.texUnit(shaderProgram,"tex0",0);
	

	//specify the background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	
	//Run until close button is pressed
	while (!glfwWindowShouldClose(window)) {
		//Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean teh back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

		//Tell OpenGl which shader program we want to use
		shaderProgram.Activate();
		//Assign a new value to a uniform ;Note this must always be done after activating the shader program
		glUniform1f(uniID,0.5f);
		//bind the texture
		manTexture.Bind();

		//Bind the VAO so that OpenGL knows to use it
		VAO1.Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		//Draw Primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		//Take care of all glfw events
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	manTexture.Delete();
	shaderProgram.Delete();
	

	//Delete the window before closing the program
	glfwDestroyWindow(window);
	//Close the program
	glfwTerminate();


	return 0;
}