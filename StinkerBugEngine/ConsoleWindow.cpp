#include "ConsoleWindow.h"



void ConsoleWindow::Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs) {
	//ImGui::SetNextWindowPos(ImVec2(350, display.windowHeight - 300));
	//ImGui::SetNextWindowSize(ImVec2(display.windowWidth - 700, 300));
	ImGui::SetNextWindowPos(ImVec2(350, display.windowHeight - 300));
	ImGui::SetNextWindowSize(ImVec2((display.windowWidth - 700) * 0.5f, 300));
	ImGui::Begin("Console", &opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	const auto& lines = consoleCapture.GetLines();
	for (const auto& line : lines) {
		ImGui::TextUnformatted(line.c_str());
	}
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);


	ImGui::End();
}