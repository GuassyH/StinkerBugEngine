#include "SceneViewWindow.h"

#include "ComponentsList.h"
#include "EntityHelper.h"


inline void DebugVect3(std::string text, glm::vec3 vector) {
	std::cout << text << ": " << vector.x << "x " << vector.y << "y " << vector.z << "z\n";
}

void SceneViewWindow::Init() {
	cam_output = new Texture();

	editorCamera = new EditorCamera();
	editorCamera->transform = new Transform();
	editorCamera->transform->rotation = glm::vec3(0, 0, -1);
	editorCamera->camera = new Camera(1920, 1080, *editorCamera->transform);
	editorCamera->camera->FOVdeg = 90.0f;
	editorCamera->camera->farPlane = 1000.0f;
	editorCamera->camera->nearPlane = 0.1f;
	editorCamera->camera->output_texture = cam_output;

	editorCamera->camera->CheckOuputFBO();
}

void SceneViewWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity) {
	editorCamera->camera->UpdateMatrix(1920, 1080);
	editorCamera->camera->Render(&scene);

	DebugVect3("SceneView cam pos", editorCamera->transform->position);
	DebugVect3("SceneView cam rot", editorCamera->transform->rotation);

	ImGui::SetNextWindowPos(ImVec2(350, 0));
	ImGui::SetNextWindowSize(ImVec2(display.windowWidth - 700, display.windowHeight - 340));
	ImGui::Begin("Scene View", &opened);

	if (!editorCamera->camera->output_texture) { ImGui::End(); std::cout << "No output texture!\n"; return; }
	if (ImGui::IsWindowHovered() && glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		editorCamera->Move();
		editorCamera->Look();
	}

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 imageSize = ImVec2(cam_output->imgWidth * (windowSize.x / cam_output->imgWidth), // X
							 cam_output->imgHeight * (windowSize.x / cam_output->imgWidth)); // Y

	ImGui::Image((ImTextureID)(intptr_t)cam_output->ID, imageSize, ImVec2(0, 1), ImVec2(1, 0));



	ImGui::End();
}