#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<glfw/glfw3.h>


std::string get_file_contents(const char* filename);
GLuint create_shader(const char* sourceCode,const bool isFragmentCode);

class Shader {

	public :
		GLuint ID;
		Shader(const char* vertexFile, const char* fragmentFile);


		void Activate();
		void Delete();
	private:
		void CompileErrors(unsigned int shader, const char* type);

};


#endif


