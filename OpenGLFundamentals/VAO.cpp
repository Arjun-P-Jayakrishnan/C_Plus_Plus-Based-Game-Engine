#include "VAO.h"


VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO VBO, GLuint layout,GLuint numComponents,GLenum type,GLsizeiptr stride,void* offset) {
	VBO.Bind();
	/*
		Configure VAO so that OPenGL knows how to use it

		basically if you have first 3 values for vertices then 3 values for color

		numComponents i.e. 3 vertices  then 3 3 colors then3
		offset- gap from start vertex-0 color-3
		stride- gap between two beginings of two sets vertex - vertex or color to color
	*/
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	/*
		inorder to use it we need to enable it
		pass in position of vertex attribute
	*/
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}