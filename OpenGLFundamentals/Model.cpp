#include "Model.h"

Model::Model(const char* file) {

	//Make JSON object
	std::string text = get_file_contents(file);
	JSON = json::parse(text);

	//Get binary data
	Model::file = file;
	data = getData();

	//traverse all nodes
	traverseNode(0);
}

void Model :: Draw(Shader& shader, Camera& camera) {

	//Go over all the meshes and draw each one
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Mesh::Draw(shader,camera, matricesMeshes[i]);
	
	}
}

void Model::loadMesh(unsigned int indMesh) {
	
	//Get all accessor indices
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	//Use accessor indices to get all vertices components
	std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = groupFloatsVec3(posVec);

	std::vector<float> normVec = getFloats(JSON["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = groupFloatsVec3(normVec);

	std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
	std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);


	//Combine all the vertex components and also get the indices and textures
	std::vector<Vertex> vertices = assembleVertex(positions, normals, texUVs);
	std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
	std::vector<Texture> textures = getTextures();

	for (unsigned int i = 0; i < texUVs.size(); i++) {
		std::cout<<"texVec x" << texUVs[i].x <<"texUV y"<<texUVs[i].y << std::endl;
	}

	//Combine the vertices,indices,and textures into a mesh
	meshes.push_back(Mesh(vertices,indices,textures));
}

void Model::traverseNode(unsigned int nextNode,glm::mat4 matrix) {
	//current node
	json node = JSON["nodes"][nextNode];


	//Get translation if it exists
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end()) {
		float transValues[3];

		for (unsigned int i = 0; i < node["translation"].size(); i++) {
			transValues[i] = (node["translation"][i]);
		}

		translation = glm::make_vec3(transValues);
	
	}

	//Get Quaternion if it exists
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end()) {
		

		
		
		float rotValues[4] = {
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2],

		};
		rotation = glm::make_quat(rotValues);
	}

	//Get scale if it exists
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end()) {
		
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++) {
			scaleValues[i] = (node["scale"][i]);
		}
		scale = glm::make_vec3(scaleValues);
	}

	//Get Matrix if exists
	glm::mat4 matNode = glm::mat4(1.0f);
	if (node.find("matrix") != node.end()) {
		float matValues[16];
		for (unsigned int i = 0; i < node["matrix"].size(); i++) {
			matValues[i] = (node["matrix"][i]);
		}

		matNode = glm::make_mat4(matValues);
	}

	//Initializes the matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	//Use translation, rotation, and scale to chnage the initialized matrices
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	//Multiply all matrices together
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	//Check if the node contains a mesh and if it does load it.
	if (node.find("mesh")!= node.end()) {
		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}


	//Check if the node has children, and if it does, apply thus function to them with the matNextNode
	if (node.find("children") != node.end()) {
		for (unsigned int i = 0; i < node["children"].size(); i++) {
			traverseNode(node["children"][i], matNextNode);
		
		}
	}
}

std::vector <unsigned char> Model::getData() {

	std::string bytesText;
	//Check in buffers [0] and the at uri for the binary files
	std::string uri = JSON["buffers"][0]["uri"];

	//convert character sequence into a string
	std::string fileStr = std::string(file);

	//Get the file path from 0 index to the last index of /
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	//From file path go to the bin and get the data contents
	/*
		Eventhough a model like glb is a singular file it has inner files
		which may have a bin file
	*/
	bytesText = get_file_contents((fileDirectory+uri).c_str());


	std::vector <unsigned char> data(bytesText.begin(), bytesText.end());

	return data;
}

std::vector<float> Model::getFloats(json accessor) {

	std::vector<float> floatVec;

	unsigned int buffViewInd = accessor.value("bufferView",1);
	//Tells how many floats we have to get
	unsigned int count = accessor["count"];
	//Tells how much offset from buffer view we have to look for data
	/*
		Lets say position,normal data is in one set then whats the offset amount
	*/
	unsigned int accByteOffset = accessor.value("byteOffset",0);
	//How to group the floats
	std::string type = accessor["type"];

	json bufferView = JSON["bufferViews"][buffViewInd];
	//Tells us where that buffer view starts
	/*
		Think whats the amount between two data
	*/
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int numPerVert;
	if (type == "SCALAR")  numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Type is invalid ( not SCALAR,VEC2,VEC3,or VEC4 )");

	unsigned int beginningOfData = byteOffset + accByteOffset;
	//a byte is 4 bits 
	unsigned int lengthOfData = count * 4 * numPerVert;

	/*
		Structure is byteOffset|accessorByteOffset|....data....|
	*/
	for (unsigned int i = beginningOfData; i < beginningOfData+lengthOfData;i+=4) {
		unsigned char bytes[] = { data[i],data[i+1],data[i+2],data[i+3] };
		
		//To store the float value from bytes
		float value;

		//Converts byte to float
		std::memcpy(&value, bytes, sizeof(float));

		//pushes it into the float array
		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor) {

	std::vector<GLuint> indices;

	unsigned int buffViewInd = accessor.value("bufferView", 0);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset",0);
	unsigned int componentType = accessor["componentType"];

	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int beginningOfData = byteOffset + accByteOffset;
	/*
		5125 -> value if its unsigned integer
		5123 -> value if its unsigned short
		5122 -> value if its short
	*/
	if (componentType == 5125) {
		
		for (unsigned int i = beginningOfData; i < beginningOfData + count * 4; i+=4) {
			
			unsigned char bytes[] = {data[i],data[i+1],data[i+2],data[i+3]};

			unsigned int value;

			std::memcpy(&value, bytes,sizeof(unsigned int));

			indices.push_back((GLuint)value);
		
		}
	}
	else if (componentType == 5123) {
	
		for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i+=2) {
			
			unsigned char bytes[] = { data[i],data[i+1] };

			unsigned short value;

			std::memcpy(&value, bytes, sizeof(unsigned short));

			indices.push_back((GLuint)value);
		
		}
	}
	else if (componentType == 5122) {
		
		for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i+=2) {
			
			unsigned char bytes[] = {data[i],data[i+1]};

			short value;

			std::memcpy(&value, bytes, sizeof(short));

			indices.push_back((GLuint)value);
		
		}
	}
	return indices;
}

std::vector <Vertex>  Model::assembleVertex(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
) {

	std::vector<Vertex> vertices;
	for (int i = 0; i < positions.size(); i++) {
		
		vertices.push_back(
			Vertex{
				positions[i],
				normals[i],
				glm::vec3(1.0f,1.0f,1.0f),
				texUVs[i]
			}
		);
	
	}
	
	return vertices;
}

std::vector<Texture> Model::getTextures() {
	
	std::vector<Texture> textures;

	std::string fileStr = std::string(file);
	std::string fileDirectory = fileStr.substr(0,fileStr.find_last_of('/')+1);
	

	
	for (unsigned int i = 0; i < JSON["images"].size(); i++) {

		std::string texPath = JSON["images"][i]["uri"];

		bool skip = false;
		for (unsigned int j = 0; j < loadedTexName.size(); j++) {
		
			if (loadedTexName[j] == texPath) {
				//we dont recreate the texture just add as duplicate
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			if (texPath.find("baseColor") != std::string::npos) {

				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
			}
			else if (texPath.find("metallicRoughness") != std::string::npos) {

				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size());
				textures.push_back(specular);
				loadedTex.push_back(specular);
				loadedTexName.push_back(texPath);
			}
		}
	}

	return textures;
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector <float> floatVec) {

	const unsigned int floatsPerVector = 2;
	std::vector<glm::vec2> vectors;

	for (unsigned int i = 0; i < floatVec.size(); i+=floatsPerVector) {
		
		vectors.push_back(glm::vec2(0.0,0.0));

		for (unsigned int j = 0; j < floatsPerVector; j++) {
			
			vectors.back()[j] = floatVec[i + j];
		
		}
	}

	return vectors;
}

std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec) {


	const unsigned int floatsPerVector = 3;
	std::vector<glm::vec3> vectors;

	for (unsigned int i = 0; i < floatVec.size(); i+=floatsPerVector) {
		vectors.push_back(glm::vec3(0.0,0.0,0.0));

		for (unsigned int j = 0; j < floatsPerVector; j++) {
			vectors.back()[j] = floatVec[i + j];
		}
	}

	return vectors;
}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec) {

	const unsigned int floatsPerVector = 4;
	std::vector<glm::vec4> vectors;

	for (unsigned int i = 0; i < floatVec.size(); i+=floatsPerVector) {
		vectors.push_back(glm::vec4(0.0,0.0,0.0,0.0));

		for (unsigned int j = 0; j < floatsPerVector; j++) {
			vectors.back()[j] = floatVec[(i + j)];
		}
	}

	return vectors;
}