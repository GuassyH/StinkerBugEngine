#pragma once

#include "Scene.h"
#include "Display.h"
#include <string.h>
#include "Entity.h"

class HierarchyWindow {
private:
	bool opened = true;
	bool closed = true;

	Entity hovered_entity;

	Display& display = Display::getInstance();
public:
	void Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity);
};