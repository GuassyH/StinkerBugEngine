#include "EntitySelector.h"


void EntitySelector::Draw(Scene& scene, Entity& ID, bool& is_entity_selected, Entity& selected_entity) {
	std::ostringstream ss; ss << scene.Scene_ECS.entity_names.find(ID)->second;


	if(ImGui::Selectable(ss.str().c_str(), selected_entity == ID && is_entity_selected)) {
		if (scene.Scene_ECS.entities.find(ID) == scene.Scene_ECS.entities.end()) {
			is_entity_selected = false;
			selected_entity = ID;
		}
		else {
			selected_entity = ID;
			is_entity_selected = true;
		}
	}	
	
	double m_x, m_y;
	glfwGetCursorPos(Display::getInstance().window, &m_x, &m_y);

	if (glfwGetMouseButton(Display::getInstance().window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		if (ImGui::IsItemHovered() && !ImGui::IsPopupOpen("Change Object")) {
			selected_entity = ID;
			is_entity_selected = true;
			ImGui::OpenPopup("Change Object");
		}
	}

}