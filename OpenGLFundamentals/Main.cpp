#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Mesh.h"



Vertex vertices[] = {
			//COORDINATES						//NORMALS					//COLOR						//TEXTURE
	Vertex{ glm::vec3( - 1.0f,	0.0f,	 1.0f),	glm::vec3(0.0f, 1.0f,0.0f),	glm::vec3(0.0f,0.0f,0.0f),  glm::vec2(0.0f,0.0f),		},
	Vertex{ glm::vec3(-1.0f,	0.0f,	-1.0f), glm::vec3(0.0f, 1.0f,0.0f),	glm::vec3(0.0f,0.0f,0.0f),	glm::vec2(0.0f,1.0f),		},	
	Vertex{ glm::vec3( 1.0f,	0.0f,	-1.0f),	glm::vec3(0.0f, 1.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f),	glm::vec2(1.0f,1.0f),		},
	Vertex{ glm::vec3(1.0f,		0.0f,	 1.0f),	glm::vec3(0.0f, 1.0f,0.0f),	glm::vec3(0.0f,0.0f,0.0f),	glm::vec2(1.0f,0.0f),		}
};

GLuint indices[] = {

		0,1,2,//Bottom side
		0,2,3,//Bottom side

};

Vertex lightVertices[] = {
	//Coordinates
	Vertex{glm::vec3( - 0.1f,-0.1f, 0.1f)},
	Vertex{glm::vec3(-0.1f,-0.1f,-0.1f)},
	Vertex{glm::vec3(0.1f,-0.1f,-0.1f)},
	Vertex{glm::vec3(0.1f,-0.1f, 0.1f)},

	Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
	Vertex{glm::vec3(-0.1f, 0.1f,-0.1f)},
	Vertex{glm::vec3(0.1f, 0.1f,-0.1f)},
	Vertex{glm::vec3(0.1f, 0.1f, 0.1f)},
};

GLuint lightIndices[] = {
	0,1,2,
	0,2,3,
	0,4,7,
	0,7,3,
	3,7,6,
	3,6,2,
	2,6,5,
	2,5,1,
	1,5,4,
	1,4,0,
	4,5,6,
	4,6,7

};

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
	

	Texture textures[]{
		Texture("planks.png","diffuse",0,GL_RGBA,GL_UNSIGNED_BYTE),
		Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};


	//Generates shader object using default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

	//Create the Mesh
	Mesh floor(verts, ind, tex);


	Shader lightShader("light.vert","light.frag");
	std::vector <Vertex> vertLight(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	

	//Create Light Mesh
	Mesh lightSource(vertLight, lightInd, tex);


	glm::vec4 lightColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f,0.5f,0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 objectPos = glm::vec3(0.0f,0.0f,0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel,objectPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID,"model"),1,GL_FALSE,glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"),lightColor.x,lightColor.y,lightColor.z,lightColor.w);


	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z,lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID,"lightPos"),lightPos.x,lightPos.y,lightPos.z);

	

	

	//specify the background color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	
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
		
		floor.Draw(shaderProgram,camera);
		lightSource.Draw(lightShader, camera);
	


		//Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		//Take care of all glfw events
		glfwPollEvents();
	}

	
	shaderProgram.Delete();
	lightShader.Delete();

	

	//Delete the window before closing the program
	glfwDestroyWindow(window);
	//Close the program
	glfwTerminate();


	return 0;
}