#include "GameWindow.h"



void GameWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity) {
	// Begin Game View window
	// ImGui::SetNextWindowPos(ImVec2(350, 30));
	// ImGui::SetNextWindowSize(ImVec2(display.windowWidth - 700, display.windowHeight - 330));
	
	ImGui::SetNextWindowPos(ImVec2(350 + ((display.windowWidth - 700) * 0.5f), display.windowHeight - 300));
	ImGui::SetNextWindowSize(ImVec2((display.windowWidth - 700) * 0.5f, 300));
	ImGui::Begin("Game View", &opened, ImGuiWindowFlags_NoScrollWithMouse);

	if (!scene.HasMainCamera()) {
		ImGui::End();
		return;
	}

	Camera& camera = scene.main_camera->GetComponent<Camera>();
	if (camera.width <= 0 || camera.height <= 0) {
		ImGui::End();
		std::cout << "Camera has invalid dimensions\n";
		return;
	}

	if (!camera.output_texture) {
		camera.output_texture = new Texture(camera.width, camera.height);
		camera.CheckOuputFBO(camera.output_texture);
		std::cout << "Created texture ID: " << camera.output_texture->ID << "\n";
	}

	if (camera.output_texture->ID == 0) {
		ImGui::End();
		std::cout << "Texture not initialized properly\n";
		return;
	}


	if (!camera.output_texture) {
		camera.output_texture = new Texture(1920, 1080);
	}

	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	ImVec2 windowPos = ImGui::GetWindowPos(); // top-left of the window in screen coordinates

	// Camera aspect ratio (width / height)
	float cameraAspect = (float)camera.width / (float)camera.height;
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
	
	ImGui::Image((ImTextureID)(intptr_t)camera.output_texture->ID, imageSize, ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}