#include "EditorCamera.h"
#include "Scene.h"



void EditorCamera::AddGizmoEntities(Scene& scene) {

	arrowX = EntityHelper(scene.CreateEntity("ArrowX"), &scene.Scene_ECS);
	arrowX.AddComponent<MeshRenderer>().model = new Model(glm::vec3(0.5f));
	arrowX.GetComponent<MeshRenderer>().model->loadModel("assets/models/arrow/arrow.gltf");
	arrowX.GetComponent<MeshRenderer>().material = new Material();
	arrowX.GetComponent<MeshRenderer>().material->Color = Constants::Colors::Red;
	arrowX.GetComponent<Transform>().scale = arrowX.GetComponent<MeshRenderer>().model->model_scale;
	arrowX.GetComponent<Transform>().rotation = glm::vec3(0.0f, 0.0f, -90.0f);


	arrowY = EntityHelper(scene.CreateEntity("ArrowY"), &scene.Scene_ECS);
	arrowY.AddComponent<MeshRenderer>().model = new Model(glm::vec3(0.5f));
	arrowY.GetComponent<MeshRenderer>().model->loadModel("assets/models/arrow/arrow.gltf");
	arrowY.GetComponent<MeshRenderer>().material = new Material();
	arrowY.GetComponent<MeshRenderer>().material->Color = Constants::Colors::Green;
	arrowY.GetComponent<Transform>().scale = arrowY.GetComponent<MeshRenderer>().model->model_scale;
	arrowY.GetComponent<Transform>().rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	arrowZ = EntityHelper(scene.CreateEntity("ArrowZ"), &scene.Scene_ECS);
	arrowZ.AddComponent<MeshRenderer>().model = new Model(glm::vec3(0.5f));
	arrowZ.GetComponent<MeshRenderer>().model->loadModel("assets/models/arrow/arrow.gltf");
	arrowZ.GetComponent<MeshRenderer>().material = new Material();
	arrowZ.GetComponent<MeshRenderer>().material->Color = Constants::Colors::Blue;
	arrowZ.GetComponent<Transform>().scale = arrowZ.GetComponent<MeshRenderer>().model->model_scale;
	arrowZ.GetComponent<Transform>().rotation = glm::vec3(-90.0f, 0.0f, 0.0f);

	LittleBall = EntityHelper(scene.CreateEntity("LittleBall"), &scene.Scene_ECS);
	Mesh mesh = Mesh(Constants::Shapes::UVSphere());
	LittleBall.AddComponent<MeshRenderer>().model = new Model(mesh);
	LittleBall.GetComponent<MeshRenderer>().material = new Material();
	LittleBall.GetComponent<MeshRenderer>().material->Color = Constants::Colors::White;
	LittleBall.GetComponent<Transform>().scale = glm::vec3(0.1f);

}

void EditorCamera::DrawGizmos(Scene& scene, bool& is_entity_selected, Entity& selected_entity) {
	selected_entity_helper.ecs = &scene.Scene_ECS;
	selected_entity_helper.id = selected_entity;
	if (!is_entity_selected) { return; }

	arrowX.GetComponent<Transform>().position = selected_entity_helper.GetComponent<Transform>().position;
	arrowY.GetComponent<Transform>().position = selected_entity_helper.GetComponent<Transform>().position;
	arrowZ.GetComponent<Transform>().position = selected_entity_helper.GetComponent<Transform>().position;
	LittleBall.GetComponent<Transform>().position = selected_entity_helper.GetComponent<Transform>().position;
}