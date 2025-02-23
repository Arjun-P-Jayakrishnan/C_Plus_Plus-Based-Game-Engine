#version 330 core


layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNorm;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 scale;
uniform mat4 rotation;
uniform mat4 translation;
uniform float outlining;

void main(){

	vec3 crntPos = vec3(model*translation*rotation*scale*vec4(aPos+aNorm*outlining,1.0));
	gl_Position= camMatrix*vec4(crntPos,1.0);
	
}