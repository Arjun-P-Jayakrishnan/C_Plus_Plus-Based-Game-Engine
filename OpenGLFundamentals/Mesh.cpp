#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices,std::vector <GLuint>& indices,std::vector <Texture>& textures ) {

	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();
	//Generate the Vextex Buffer Object and link it to vertices
	VBO VBO(vertices);
	//Generate the Element Buffer Object and link with indices
	EBO EBO(indices);


	//Link Attribs such as position normal color and texCoordninates to the vbo
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3*sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6*sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT,	sizeof(Vertex), (void*)(9*sizeof(float)));


	//Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Draw(
	Shader& shader,
	Camera& camera,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
) {

	//Bind the shader to acces it contents
	shader.Activate();
	VAO.Bind();

	//Keep track of how many of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++) {
	
		std::string num;
		std::string type = textures[i].type;

		if (type == "diffuse") {
			num = std::to_string(numDiffuse++);
		}
		else if(type =="specular") {
			num = std::to_string(numSpecular++);
		}


		textures[i].texUnit(shader,(type+num).c_str(),i);
		textures[i].Bind();
	
	}
	//Take Care of the Camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");


	glm::mat4 trans = glm::mat4(1.0);
	glm::mat4 rot = glm::mat4(1.0);
	glm::mat4 sca = glm::mat4(1.0);
	
	trans= glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);


	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));



	//Draw the actual Mesh
	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
}