#include "SceneViewWindow.h"

#include "ComponentsList.h"
#include "EntityObject.h"
#include "Constants.h"
#include "SceneManager.h"
#include "Screen.h"

void SceneViewWindow::Init(ECSystem& editor_ecs) {
	cam_output = new Texture();
	editorCamera = new EditorCamera();
	editorCamera->Init();

	editorCamera->transform->position = glm::vec3(0.0f, 5.0f, 10.0f);
	editorCamera->transform->rotation = glm::vec3(0.0f, 180.0f, 0.0f);

	editorCamera->camera->output_texture = cam_output;
	// editorCamera->AddGizmoEntities(SceneManager::getInstance().GetActiveScene(), editor_ecs);
}
 

void SceneViewWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {

	// Begin SceneViewWindow
	ImGui::Begin("Scene View");

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (!window || !window->Active || !window->WasActive || window->Hidden == true)
	{
		ImGui::End();
		return;
	}

	ImGuiStyle* custom_syle = &ImGui::GetStyle();

	// Scene View Selectables
	ImGui::SetCursorPos(ImVec2(0, 18));
	ImGui::BeginChild("Toolbar", ImVec2(ImGui::GetWindowSize().x, 20), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	
	ImGui::Selectable("Grid", &editorCamera->showGrid, ImGuiSelectableFlags_None, ImVec2(30, 20));
	ImGui::SameLine();
	ImGui::Selectable("Render Shadows", &editorCamera->camera->renderShadows, ImGuiSelectableFlags_None, ImVec2(100, 20));
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 50);
	ImGui::Selectable("Stats", &showStats, ImGuiSelectableFlags_None, ImVec2(35, 20)); // SHOULD BE MENU?

	ImGui::EndChild();


	// Actual editor Camera
	editorCamera->camera->width = ImGui::GetWindowWidth();
	editorCamera->camera->height = ImGui::GetWindowHeight();
	editorCamera->Render(scene, is_entity_selected, selected_entity, editor_ecs);
	if (editorCamera->camera->output_texture ) { 

		ImVec2 windowSize = ImGui::GetContentRegionAvail();
		ImVec2 windowPos = ImGui::GetWindowPos(); // top-left of the window in screen coordinates


		ImVec2 imageSize;
		imageSize = ImGui::GetWindowSize();
		imageSize.y -= 1; // Because of padding

		ImVec2 imagePosInWindow;
		imagePosInWindow.x = ((windowSize.x - imageSize.x) * 0.5f) + ImGui::GetCursorPosX();
		imagePosInWindow.y = ((windowSize.y - imageSize.y) * 0.5f) + ImGui::GetCursorPosY();


		ImGui::SetCursorPos(ImVec2(0, 0));

		if (ImGui::IsWindowHovered() && glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
			editorCamera->w_size.x = imageSize.x;
			editorCamera->w_size.y = imageSize.y;
		
			// Due to the toolbar this is inaccurate, needs fixing
			editorCamera->w_pos.x = (windowPos.x + imagePosInWindow.x);
			editorCamera->w_pos.y = (windowPos.y + imagePosInWindow.y);

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
	
		if (showStats) {
			ImGui::SetNextWindowSize(ImVec2(220, 80));
			ImGui::SetNextWindowPos(ImVec2(windowPos.x + windowSize.x - 220, windowPos.y + imagePosInWindow.y + 18)); // simplified
			ImGui::Begin("Stats", &opened, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

			ImGui::Text("Stats");
			ImGui::Separator();

			std::ostringstream fps_text;	fps_text << display.FrameRate << " : fps";
			std::ostringstream delta_text;	delta_text << glm::round((DeltaTime::getInstance().get() * 100000.0f)) / 100.0f << " : ms";

			ImGui::TextUnformatted(fps_text.str().c_str());
			ImGui::TextUnformatted(delta_text.str().c_str());

			ImGui::End();
		}
	}
	else {
		ImGui::End(); std::cout << "No output texture!\n";
	}

	if (glfwGetKey(display.window, GLFW_KEY_F) == GLFW_PRESS) {
		if (ImGui::IsWindowHovered() && glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
			editorCamera->transform->position = scene.Scene_ECS.GetComponent<Transform>(selected_entity).position - (editorCamera->camera->forward * 5.0f);
		}
	}

	if (ImGui::IsWindowHovered()) {
		editorCamera->SelectObject(scene, is_entity_selected, selected_entity, editor_ecs);
	}



	ImGui::End();
}


