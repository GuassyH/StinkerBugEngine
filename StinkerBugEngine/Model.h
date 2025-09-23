#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <iostream>
#include <vector>
#include "glfw/glfw3.h"	

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "Constants.h"
#include "Vertex.h"

#include "Texture.h"
#include "Mesh.h"

class Model {
protected:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;


	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadTextures(aiMaterial* materials, aiTextureType type);
public:
	const char* name = "new_model";

	Model() = default;
	~Model() = default;

	void init();
	void loadModel(std::string path);
	void render();
	void cleanup();
};


#endif