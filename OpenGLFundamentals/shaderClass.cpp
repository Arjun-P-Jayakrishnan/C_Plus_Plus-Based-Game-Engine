#include"shaderClass.h"

std::string get_file_contents(const char* filename) {

	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}

	throw(errno);
}


GLuint create_shader(const char* sourceCode,const bool isFragment) {
	
	//Create Vertex/Fragment Shader Object and getting its reference
	GLuint shader;

	if (isFragment == true) {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else {
		shader = glCreateShader(GL_VERTEX_SHADER);
	}

	//Attach Vertex Shader source to Vertex Shader Object
	glShaderSource(shader, 1, &sourceCode, NULL);
	//Compile the Vertex Shader into machine code
	glCompileShader(shader);


	return shader;
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {

	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();


	GLuint vertexShader = create_shader(vertexSource,false);
	CompileErrors(vertexShader, "VERTEX");
	GLuint fragmentShader = create_shader(fragmentSource, true);
	CompileErrors(fragmentShader, "FRAGMENT");


	//Create Shader Program Object and get its reference
	ID = glCreateProgram();
	//Attach Vertex and Fragment Shaders to the shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	//Wrap up/Link all the shaders together into ShaderProgram
	glLinkProgram(ID);

	//The shader code is already in the program so can be deleted
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::Activate() {

	glUseProgram(ID);
	
}

void Shader::Delete() {

	glDeleteProgram(ID);

}

void Shader::CompileErrors(unsigned int shader, const char* type) {

	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS,&hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader,1.024,NULL,infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for :" << type << "\n" << std::endl;
		}
	}
	else {
	
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
		}
	}

}