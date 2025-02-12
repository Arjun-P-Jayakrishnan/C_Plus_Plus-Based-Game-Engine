#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "Camera.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "VBO.h"

class Mesh {

public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	//Store in public as it can be used in the Draw function
	VAO VAO;

	//Initializes the mesh
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	//Draws the mesh
	void Draw(Shader& shader,Camera& camera);
};

#endif