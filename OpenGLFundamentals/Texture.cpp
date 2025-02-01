#include "Texture.h"



Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
	int widthImg, heightImg, numColCh;
	
	type = texType;
	unsigned char* bytes = load_image(image, &widthImg, &heightImg, &numColCh);
	
	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	free_image(bytes);
	glBindTexture(texType, 0);


}


unsigned char* Texture::load_image(const char* fileName, int* widthImg, int* heightImg, int* numColCh) {
	/*
		OpenGL reads from topleft to bottomright whereas
		STBI reads from bottomright to topleft
		so we need to flip the images
	*/
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(fileName, widthImg, heightImg, numColCh, 0);

	return bytes;
}

void Texture::free_image(unsigned char* imageBytes) {

	stbi_image_free(imageBytes);
}


void Texture::texUnit(Shader shader, const char* uniform, GLuint uint) {
	
	GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(tex0Uni, uint);
}

void Texture::Bind() {

	glBindTexture(type,ID);
}

void Texture::Unbind() {
	
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}