#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "json/json.h"
#include "Mesh.h"

using json = nlohmann::json;

class Model {

public:
	Model(const char* file);

	void Draw(Shader& shader,Camera& camera);

private:

	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	std::vector<Mesh> meshes;

	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;



	std::vector <std::string> loadedTexName;
	std::vector <Texture> loadedTex;

	void loadMesh(unsigned int indMesh);
	void traverseNode(unsigned int nextNode,glm::mat4 matrix = glm::mat4(1.0));

	std::vector<unsigned char> getData();

	//Basically gets the positions normals and texUVs
	std::vector<float> getFloats(json accessor);
	//Gets the indices
	std::vector<GLuint> getIndices(json accessor);
	//Gets the textures
	std::vector<Texture> getTextures();

	/*
		converts the position normals texUVs into a vertex object 
		and makes the entrire set as a vertex object list
	*/
	std::vector<Vertex> assembleVertex(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	
	);
	
	// converts the list from float array to vec2 array by grouping 2 floats into one vec2
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	//converts the list from floats array to vec3 array by grouping 3 floats into one vec3
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	//converts the list from floats
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

#endif