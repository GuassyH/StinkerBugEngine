#include "Model.h"


bool Model::LoadModel() {
	if (!isValid()) { std::cout << "Failed to load model: " << model_path << "\n"; return false; }



	return true;
}



bool Model::isValid() {
	if (std::string(model_path) == "none") { return false; }
	else { return true; }
}


std::vector<Vertex>& Model::getVertices() { return vertices; }
std::vector<GLuint>& Model::getIndices() { return indices; }