#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <iostream>
#include <vector>
#include "glfw/glfw3.h"	

#include "Constants.h"
#include "Vertex.h"

class Model {
private:
	std::vector<Vertex> vertices = {
        { {-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1.0f, 0.0f} }, // Left Bottom Back
        { {-0.5f,  0.5f, -0.5f}, {0, 0, -1}, {1.0f, 1.0f} }, // Left Top Back
        { { 0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0.0f, 0.0f} }, // Right Bottom Back
        { { 0.5f,  0.5f, -0.5f}, {0, 0, -1}, {0.0f, 1.0f} }, // Right Top Back

        { {-0.5f, -0.5f,  0.5f}, {0, 0,  1}, {0.0f, 0.0f} }, // Left Bottom Front
        { { 0.5f, -0.5f,  0.5f}, {0, 0,  1}, {1.0f, 0.0f} }, // Right Bottom Front
        { {-0.5f,  0.5f,  0.5f}, {0, 0,  1}, {0.0f, 1.0f} }, // Left Top Front
        { { 0.5f,  0.5f,  0.5f}, {0, 0,  1}, {1.0f, 1.0f} }, // Right Top Front

        { {-0.5f,  0.5f, -0.5f}, {0, 1,  0}, {0.0f, 1.0f} }, // Left Top Back
        { {-0.5f,  0.5f,  0.5f}, {0, 1,  0}, {0.0f, 0.0f} }, // Left Top Front
        { { 0.5f,  0.5f, -0.5f}, {0, 1,  0}, {1.0f, 1.0f} }, // Right Top Back
        { { 0.5f,  0.5f,  0.5f}, {0, 1,  0}, {1.0f, 0.0f} }, // Right Top Front

        { {-0.5f, -0.5f, -0.5f}, {0,-1,  0}, {0.0f, 0.0f} }, // Left Bottom Back
        { { 0.5f, -0.5f, -0.5f}, {0,-1,  0}, {1.0f, 0.0f} }, // Right Bottom Back
        { {-0.5f, -0.5f,  0.5f}, {0,-1,  0}, {0.0f, 1.0f} }, // Left Bottom Front
        { { 0.5f, -0.5f,  0.5f}, {0,-1,  0}, {1.0f, 1.0f} }, // Right Bottom Front

        { {-0.5f, -0.5f, -0.5f}, {-1, 0,  0}, {0.0f, 0.0f} }, // Left Bottom Back
        { {-0.5f, -0.5f,  0.5f}, {-1, 0,  0}, {1.0f, 0.0f} }, // Left Bottom Front
        { {-0.5f,  0.5f, -0.5f}, {-1, 0,  0}, {0.0f, 1.0f} }, // Left Top Back
        { {-0.5f,  0.5f,  0.5f}, {-1, 0,  0}, {1.0f, 1.0f} }, // Left Top Front

        { { 0.5f, -0.5f, -0.5f}, { 1, 0,  0}, {1.0f, 0.0f} }, // Right Bottom Back
        { { 0.5f,  0.5f, -0.5f}, { 1, 0,  0}, {1.0f, 1.0f} }, // Right Top Back
        { { 0.5f, -0.5f,  0.5f}, { 1, 0,  0}, {0.0f, 0.0f} }, // Right Bottom Front
        { { 0.5f,  0.5f,  0.5f}, { 1, 0,  0}, {0.0f, 1.0f} }, // Right Top Front
	};
	std::vector<GLuint> indices = {
        // Back face
    0, 1, 2,
    2, 1, 3,

    // Front face
    4, 5, 6,
    6, 5, 7,

    // Top face
    8,  9, 10,
    10, 9, 11,

    // Bottom face
    12, 13, 14,
    14, 13, 15,

    // Left face
    16, 17, 18,
    18, 17, 19,

    // Right face
    20, 21, 22,
    22, 21, 23
    };
public:
	const char* model_path = "none";
	const char* name = "new_model";

	Model() = default;
	Model(const char* path) : model_path(path) {
		std::cout << "Model path: " << model_path << "\n";
	}
	~Model() = default;


    bool LoadModel();
	bool isValid();
	std::vector<Vertex>& getVertices();
	std::vector<GLuint>& getIndices();

};


#endif