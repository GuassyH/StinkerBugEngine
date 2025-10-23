#include "ConsoleWindow.h"



void ConsoleWindow::Draw() {
	ImGui::Begin("Console");
	ImGui::Spacing();

	const char* prefix = "";

	const auto& lines = consoleCapture.GetLines();
	for (const auto& line : lines) {
		ImGui::Text(prefix);
		ImGui::SameLine();
		ImGui::TextUnformatted(line.c_str());
	}
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);


	ImGui::End();
}