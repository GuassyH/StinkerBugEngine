#ifndef MESH_RENDERER_COMPONENT_H
#define MESH_RENDERER_COMPONENT_H

#include <iostream>
#include <vector>

#include "Constants.h"

#include "Material.h"
#include "Vertex.h"
#include "Mesh.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

struct MeshRenderer{
	Mesh* mesh = nullptr;
	Material* material = nullptr;
};


// Constructor for vector (dynamic size)
inline MeshRenderer CreateMeshRenderer(Mesh& mesh, Material& material)
{
	MeshRenderer mr;
	mr.mesh = &mesh;
	mr.material = &material;
	return mr;
}


#endif