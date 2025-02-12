#include "EBO.h"

EBO::EBO(std::vector <GLuint>& indices) {

	//Generate the Buffer for storing indices
	glGenBuffers(1, &ID);
	//Binds the ID to the ELEMENTS_RRAY_BUFFER_TYPE i.e. to store indices Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	//initializes the buffer with the data 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

}

void EBO::Bind() {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	glDeleteBuffers(1,&ID);
}