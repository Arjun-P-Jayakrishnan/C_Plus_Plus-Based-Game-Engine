#include "VBO.h"

VBO::VBO(std::vector <Vertex>& vertices) {

	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	/*
		STREAM  - Modified once use few time
		STATIC  - Modified once and used many times
		DYNAMIC - Modified many times and used many times

		DRAW -  use vertices to draw
		COPY
		READ -
	*/


	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}