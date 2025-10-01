#include "SceneViewWindow.h"

#include "ComponentsList.h"
#include "EntityHelper.h"
#include "Constants.h"
#include "SceneManager.h"
#include "Screen.h"

void SceneViewWindow::Init(ECSystem& editor_ecs) {
	cam_output = new Texture();
	editorCamera = new EditorCamera();
	editorCamera->Init();

	editorCamera->transform->position = glm::vec3(0.0f, 5.0f, 10.0f);

	editorCamera->camera->output_texture = cam_output;
	editorCamera->AddGizmoEntities(SceneManager::getInstance().GetActiveScene(), editor_ecs);
}
 

void SceneViewWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {
	std::ostringstream fps_text;	fps_text << display.FrameRate << "fps";


	// Begin SceneViewWindow
	ImGui::SetNextWindowPos(ImVec2(350, 30));
	ImGui::SetNextWindowSize(ImVec2(display.windowWidth - 700, display.windowHeight - 330));
	ImGui::Begin("Scene View", &opened, ImGuiWindowFlags_NoScrollWithMouse);


	// Scene View Selectables
	ImGui::SetCursorPos(ImVec2(0, 22));
	ImGui::BeginChild("Toolbar", ImVec2(ImGui::GetWindowSize().x, 20), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
	ImGui::Selectable("Grid", &editorCamera->showGrid, ImGuiSelectableFlags_None, ImVec2(30, 20));
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 50);
	ImGui::Selectable("Stats", &showStats, ImGuiSelectableFlags_None, ImVec2(35, 20)); // SHOULD BE MENU?

	ImGui::EndChild();


	// Actual editor Camera
	editorCamera->Render(scene, is_entity_selected, selected_entity, editor_ecs);
	if (editorCamera->camera->output_texture ) { 

		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		ImVec2 windowPos = ImGui::GetWindowPos(); // top-left of the window in screen coordinates

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

		ImVec2 imagePosInWindow;
		imagePosInWindow.x = ((windowSize.x - imageSize.x) * 0.5f) + ImGui::GetCursorPosX();
		imagePosInWindow.y = ((windowSize.y - imageSize.y) * 0.5f) + ImGui::GetCursorPosY();


		ImGui::SetCursorPosX(imagePosInWindow.x);
		ImGui::SetCursorPosY(imagePosInWindow.y);

		if (ImGui::IsWindowHovered() && glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
			editorCamera->w_size.x = imageSize.x;
			editorCamera->w_size.y = imageSize.y;
		
			editorCamera->w_pos.x = windowPos.x + imagePosInWindow.x;
			editorCamera->w_pos.y = windowPos.y + imagePosInWindow.y;

			if (firstRightClick) {
				glfwSetCursorPos(display.window, glm::roundEven((imageSize.x / 2.0f) + windowPos.x + imagePosInWindow.x), glm::roundEven((imageSize.y / 2.0f) + windowPos.y + imagePosInWindow.y));
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

		ImGui::Image((ImTextureID)(intptr_t)cam_output->ID, imageSize, ImVec2(0, 1), ImVec2(1, 0));
	
	}
	else {
		ImGui::End(); std::cout << "No output texture!\n";
	}

	if (glfwGetKey(display.window, GLFW_KEY_F) == GLFW_PRESS) {
		if (ImGui::IsWindowHovered() && glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
			editorCamera->transform->position = 
				scene.Scene_ECS.GetComponent<Transform>(selected_entity).position - (editorCamera->camera->forward * 5.0f);
		}
	}

	ImGui::End();
}


