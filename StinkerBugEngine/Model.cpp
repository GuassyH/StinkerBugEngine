#include "Model.h"

Model::Model(Mesh& mesh) {
	meshes.push_back(mesh);
}

Model::Model(const Constants::Shapes::Shape& shape) {
	Mesh mesh(shape);
	meshes.push_back(mesh);
}


void Model::init() {}


std::vector<Texture> Model::loadTextures(aiMaterial* material, aiTextureType type) {
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
		aiString str;
		material->GetTexture(type, i, &str);
		std::cout << str.C_Str() << std::endl;

		// prevent duplicates
		bool skip = false;
		
		for (unsigned int j = 0; j < textures.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			// not loaded
			Texture tex(directory, str.C_Str(), type);
			tex.Load(false);
			textures.push_back(tex);
			textures_loaded.push_back(tex);
		}
	}

	return textures;
}


void Model::loadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		std::cout << "Could not load model at " << path << std::endl << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of("/"));
	processNode(scene->mRootNode, scene);

	name = "new_Model";
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// process all meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// process child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++){
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	// vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		
		// Position
		vertex.pos = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		// Normal
		vertex.normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		// Textures
		if (mesh->mTextureCoords[0]) {
			vertex.texCoords = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else {
			vertex.texCoords = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}


	// indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}


	// Texture
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// diffuse
		std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// specular
		std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	
	Mesh newMesh(vertices, indices, textures);
	newMesh.name = "new_Mesh";
	return newMesh;
}

void Model::shadowPass(Material* material) {
	for (Mesh mesh : meshes) {
		mesh.shadowPass(material);
	}
}

void Model::render(Material* material, Transform* m_transform, Transform* c_transform, Camera* cam, Light* light) {
	for (Mesh mesh : meshes) {
		mesh.render(material, m_transform, c_transform, cam, light);
	}
}


void Model::cleanup() {
	for (Mesh& mesh : meshes) {
		mesh.cleanup();
	}
}