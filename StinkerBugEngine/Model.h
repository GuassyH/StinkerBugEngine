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
#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "Transform.h"
#include "Light.h"

class Model {
protected:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;


	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType type);
public:
	const char* name = "new_model";
	glm::vec3 model_scale = glm::vec3(1.0f);


	Model() = default;
	Model(Mesh& mesh);
	Model(const Constants::Shapes::Shape& shape);
	Model(glm::vec3 scale) : model_scale(scale) {}
	~Model() = default;

	void init();
	void loadModel(std::string path);
	// void render(Material* m_material, Transform* m_transform, Transform* c_transform, Camera* cam, Light* light, bool shadowPass);
	void shadowPass();
	void render(std::shared_ptr<Material> material, std::shared_ptr<Transform> m_transform, std::shared_ptr<Transform> c_transform, Camera* cam, Light* light);
	void cleanup();

	std::vector<Mesh>& getMeshes() { return meshes; }
	std::string getDirectory() { return directory; }
};


#endif