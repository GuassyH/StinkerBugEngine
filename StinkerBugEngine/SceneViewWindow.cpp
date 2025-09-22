#include "SceneViewWindow.h"

#include "ComponentsList.h"
#include "EntityHelper.h"
#include "Constants.h"

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

	editorCamera->camera->CheckOuputFBO(true);
}


void SceneViewWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity) {
	editorCamera->camera->UpdateMatrix(editorCamera->camera->width, editorCamera->camera->height);
	editorCamera->camera->Render(&scene);

	// Constants::DebugLog::Vec3("SceneView cam pos", editorCamera->transform->position);
	// Constants::DebugLog::Vec3("SceneView cam rot", editorCamera->transform->rotation);

	ImGui::SetNextWindowPos(ImVec2(350, 0));
	ImGui::SetNextWindowSize(ImVec2(display.windowWidth - 700, display.windowHeight - 340));
	ImGui::Begin("Scene View", &opened);

	if (!editorCamera->camera->output_texture) { ImGui::End(); std::cout << "No output texture!\n"; return; }
	if (ImGui::IsWindowHovered() && glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		editorCamera->r_size.x = ImGui::GetWindowSize().x;
		editorCamera->r_size.y = ImGui::GetWindowSize().y;
		
		editorCamera->r_pos.x = ImGui::GetWindowPos().x;
		editorCamera->r_pos.y = ImGui::GetWindowPos().y;

		if (firstRightClick) {
			glfwSetCursorPos(display.window, ((editorCamera->r_size.x / 2.0f) + editorCamera->r_pos.x), ((editorCamera->r_size.y / 2.0f) + editorCamera->r_pos.y));
		}
		glfwSetInputMode(display.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);

		editorCamera->Move();
		editorCamera->Look();

		firstRightClick = false;
	}
	else { 
		glfwSetInputMode(display.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		firstRightClick = true;
	}

	ImVec2 windowSize = ImGui::GetContentRegionAvail();

	// Camera aspect ratio (width / height)
	float cameraAspect = (float)editorCamera->camera->width / (float)editorCamera->camera->height;
	float windowAspect = windowSize.x / windowSize.y;

	ImVec2 imageSize;

	if (windowAspect > cameraAspect) {
		// Window is wider than camera -> match height
		imageSize.y = windowSize.y;
		imageSize.x = windowSize.y * cameraAspect;
	}
	else {
		// Window is taller than camera -> match width
		imageSize.x = windowSize.x;
		imageSize.y = windowSize.x / cameraAspect;
	}

	ImGui::SetCursorPosX((windowSize.x - imageSize.x) * 0.5f + ImGui::GetCursorPosX());
	ImGui::SetCursorPosY((windowSize.y - imageSize.y) * 0.5f + ImGui::GetCursorPosY());

	ImGui::Image((ImTextureID)(intptr_t)cam_output->ID, imageSize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}