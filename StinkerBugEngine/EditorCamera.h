#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H


#include "Transform.h"
#include "MeshRenderer.h"

#include "Camera.h"
#include "Constants.h"
#include "Display.h"
#include "EntityObject.h"
#include "Gizmos.h"
#include "Physics.h"
#include "ECSystem.h"

class Scene;

class EditorCamera {
private:
	unsigned int select_fbo = 0;
	unsigned int select_rbo = 0;
	unsigned int select_tex = 0;
	
	bool firstLeftClick = true;
	bool firstRightClick = true;
	bool interactingWithGizmo = false;
	Physics::RaycastHit rayHit;
public:
	std::shared_ptr<Transform> transform = nullptr;
	Camera* camera = nullptr;
	
	EditorCamera() = default;
	
	unsigned int selected_gizmo = 0;
	EntityObject selected_entity_helper;
	std::vector<Gizmos::Gizmo> pre_pass_gizmos = {};
	std::vector<Gizmos::Gizmo> post_pass_gizmos = {};

	bool showGrid = false;

	void Init();
	// TEMPORARY SOLUTION
	void AddGizmoEntities(Scene& scene, ECSystem& editor_ecs);

	void PrePass(Scene& scene, ECSystem& editor_ecs);
	void Render(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs);
	void PostPass(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs);
	void SelectObject(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs);
	

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