#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H


#include "Transform.h"
#include "MeshRenderer.h"

#include "Camera.h"
#include "Constants.h"
#include "Display.h"
#include "EntityHelper.h"
#include "Gizmos.h"

class Scene;

class EditorCamera {
private:
	unsigned int select_fbo;
	unsigned int select_rbo;
	unsigned int select_tex;

public:
	Transform* transform = nullptr;
	Camera* camera = nullptr;
	
	EditorCamera() = default;
	
	
	EntityHelper selected_entity_helper;
	unsigned int selected_gizmo = 0;
	std::vector<Gizmos::Gizmo> gizmos;


	bool firstClick = true;

	void Init();
	// TEMPORARY SOLUTION
	void AddGizmoEntities(Scene& scene);
	void DrawGizmos(Scene& scene, bool& is_entity_selected, Entity& selected_entity);
	void SelectObject(Scene& scene, bool& is_entity_selected, Entity& selected_entity);
	

	float moveSpeed = 2.0f;
	float speedMul = 1.0f;

	float sensitivity = 2.0f;

	float horizontal = 0.0f;
	float vertical = 0.0f; 
	float elevator = 0.0f;

	glm::vec3 moveDir = glm::vec3(0.0f);

	glm::vec2 w_size = glm::vec2(0.0f);
	glm::vec2 w_pos = glm::vec2(0.0f);
	void Move();
	void Look();
};



#endif