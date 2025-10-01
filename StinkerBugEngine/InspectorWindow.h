#pragma once

#include "Scene.h"
#include "Display.h"
#include "Entity.h"
#include "ECSystem.h"

class InspectorWindow {
private:
	bool opened = true;
	bool closed = false;

	Display& display = Display::getInstance();
public:
	void Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs);
};