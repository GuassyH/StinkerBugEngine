#pragma once

#include "Scene.h"
#include "Display.h"
#include "Entity.h"

class InspectorWindow {
private:
	bool opened = true;
	bool closed = false;
public:
	void Draw(Scene& scene, Display& display, bool& is_entity_selected, Entity& selected_entity);
};