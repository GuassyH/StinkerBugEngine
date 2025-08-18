#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "Display.h"
#include "Constants.h"
#include "SceneManager.h"
#include "Scene.h"
#include "FullScreenPass.h"

#include "Entity.h"

#include "Mesh.h"
#include "MeshRenderer.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "EntityBehavour.h"
#include "SphereMove.h"


int main(void) {
	Display& display = Display::getInstance();
	if (display.init(1920, 1080, "Stinker Bug Engine") == -1) { return -1; }

	Camera camera = Camera(display.windowWidth, display.windowHeight, glm::vec3(0.0));
	camera.transform.position = glm::vec3(0, 3, 10.0f);

	DeltaTime& deltaTime = DeltaTime::getInstance();
	SceneManager& sceneManager = SceneManager::getInstance();
	Scene scene;

	sceneManager.SetActiveScene(scene);

	Shader skybox_shader("skybox.vert", "skybox.frag");

	Material skybox_mat(skybox_shader);
	Material material;
	Material red; red.Color = Constants::Colors::Red;

	

	Mesh floor = Mesh(Constants::Shapes::Plane());
	Mesh cube = Mesh(Constants::Shapes::Cube());
	Mesh sphere = Mesh(Constants::Shapes::UVSphere());

	FullScreenPass skybox_pass = FullScreenPass(camera, skybox_mat);

	Entity e_floor = scene.CreateEntity();
	scene.AddComponent<MeshRenderer>(e_floor, floor, material);
	scene.AddComponent<RigidBody>(e_floor).isKinematic = true;
	scene.GetComponent<Transform>(e_floor).position = glm::vec3(-5.0, 0.0, -5.0);
	scene.GetComponent<Transform>(e_floor).scale = glm::vec3(10.0);

	Entity e_globe = scene.CreateEntity();
	scene.AddComponent<MeshRenderer>(e_globe, sphere, material);
	scene.AddComponent<RigidBody>(e_globe).useGravity = true;
	scene.GetComponent<Transform>(e_globe).position = glm::vec3(1.0, 2, 0.0);
	scene.AddComponent<SphereCollider>(e_globe).radius = 0.5f; \
	scene.AddComponent<SphereMove>(e_globe, scene.GetComponent<Transform>(e_globe), scene.GetComponent<RigidBody>(e_globe));

	Entity e_globe2 = scene.CreateEntity();
	scene.AddComponent<MeshRenderer>(e_globe2, sphere, material);
	scene.AddComponent<RigidBody>(e_globe2).useGravity = true;
	scene.GetComponent<Transform>(e_globe2).position = glm::vec3(-1.0, 2, 0.0);
	scene.AddComponent<SphereCollider>(e_globe2).radius = 0.5f;

	// SphereMove move(scene.GetComponent<Transform>(e_globe), scene.GetComponent<RigidBody>(e_globe));

	scene.StartEntityBehaviours();
	scene.WakeEntityBehaviours();
	bool shot = false;
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();
		skybox_pass.Draw(camera);


		if (glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			if (shot == false) {
				shot = true;
				Entity bullet = scene.CreateEntity();
				scene.AddComponent<MeshRenderer>(bullet, sphere, red);
				scene.GetComponent<Transform>(bullet).scale = glm::vec3(0.2);
				scene.GetComponent<Transform>(bullet).position = camera.transform.position;
				scene.AddComponent<SphereCollider>(bullet).radius = 0.1f;
				scene.AddComponent<RigidBody>(bullet).useGravity = false;
				scene.GetComponent<RigidBody>(bullet).velocity = camera.forward;
				scene.GetComponent<RigidBody>(bullet).drag = 0.0;
			}
		}
		else { shot = false; }


		camera.UpdateMatrix(75.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);
		camera.Input();

		scene.UpdateEntityBehaviours();
		scene.UpdatePhysics();
		scene.DrawMeshes(camera);


		display.EndFrame();
	}

	

	return 0;
}