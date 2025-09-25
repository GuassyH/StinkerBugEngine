#include "EditorCamera.h"
#include "Scene.h"



void EditorCamera::AddGizmoEntities(Scene& scene) {

	arrowX.mesh_renderer = MeshRenderer();
	arrowY.mesh_renderer = MeshRenderer();
	arrowZ.mesh_renderer = MeshRenderer();
	LittleBall.mesh_renderer = MeshRenderer();


	arrowX.transform = Transform();
	arrowY.transform = Transform();
	arrowZ.transform = Transform();
	LittleBall.transform = Transform();


	arrowX.mesh_renderer.model = new Model();
	arrowX.mesh_renderer.model->loadModel("assets/models/arrow/arrow.gltf");
	arrowX.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
	arrowX.mesh_renderer.material->Color = Constants::Colors::Red;
	arrowX.transform.scale = glm::vec3(0.5f);

	arrowY.mesh_renderer.model = new Model();
	arrowY.mesh_renderer.model->loadModel("assets/models/arrow/arrow.gltf");
	arrowY.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
	arrowY.mesh_renderer.material->Color = Constants::Colors::Green;
	arrowY.transform.scale = glm::vec3(0.5f);

	arrowZ.mesh_renderer.model = new Model();
	arrowZ.mesh_renderer.model->loadModel("assets/models/arrow/arrow.gltf");
	arrowZ.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
	arrowZ.mesh_renderer.material->Color = Constants::Colors::Blue;
	arrowZ.transform.scale = glm::vec3(0.5f);


	Mesh origo_mesh = Mesh(Constants::Shapes::Cube());
	LittleBall.mesh_renderer.model = new Model(origo_mesh);
	LittleBall.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
	LittleBall.transform.scale = glm::vec3(0.1f);
}

void EditorCamera::DrawGizmos(Scene& scene, bool& is_entity_selected, Entity& selected_entity) {
	// Rebind the framebuffer to the editor camera's FBO
	glBindFramebuffer(GL_FRAMEBUFFER, camera->outputFBO);

	selected_entity_helper.ecs = &scene.Scene_ECS;
	selected_entity_helper.id = selected_entity;

	// If there isnt an entity selected then skip drawing gizmos, reset framebuffer 
	if (!is_entity_selected || scene.Scene_ECS.entities.find(selected_entity) == scene.Scene_ECS.entities.end()) { glBindFramebuffer(GL_FRAMEBUFFER, 0); return; }

	glClear(GL_DEPTH_BUFFER_BIT);

	arrowX.transform.position = selected_entity_helper.GetComponent<Transform>().position;
	arrowY.transform.position = selected_entity_helper.GetComponent<Transform>().position;
	arrowZ.transform.position = selected_entity_helper.GetComponent<Transform>().position;
	LittleBall.transform.position = selected_entity_helper.GetComponent<Transform>().position;

	glm::vec3 rot = selected_entity_helper.GetComponent<Transform>().rotation;

	// unsure why -rot.z is needed for arrowZ to not be tilted weirdly
	arrowX.transform.rotation = rot + glm::vec3(0.0f, 0.0f, -90.0f);
	arrowY.transform.rotation = rot + glm::vec3(0.0f, 0.0f, 0.0f);
	arrowZ.transform.rotation = rot + glm::vec3(90.0f, 0.0f, -rot.z);

	arrowX.transform.UpdateMatrix();
	arrowY.transform.UpdateMatrix();
	arrowZ.transform.UpdateMatrix();
	LittleBall.transform.UpdateMatrix();

	arrowX.mesh_renderer.model->render(arrowX.mesh_renderer.material, &arrowX.transform, transform, camera, &scene.main_light->GetComponent<Light>(), false);
	arrowY.mesh_renderer.model->render(arrowY.mesh_renderer.material, &arrowY.transform, transform, camera, &scene.main_light->GetComponent<Light>(), false);
	arrowZ.mesh_renderer.model->render(arrowZ.mesh_renderer.material, &arrowZ.transform, transform, camera, &scene.main_light->GetComponent<Light>(), false);
	LittleBall.mesh_renderer.model->render(LittleBall.mesh_renderer.material, &LittleBall.transform, transform, camera, &scene.main_light->GetComponent<Light>(), false);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}