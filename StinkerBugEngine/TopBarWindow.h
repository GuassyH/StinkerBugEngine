#pragma once

#include "Scene.h"
#include "Display.h"
#include "Entity.h"

class TopBarWindow {
private:
	bool opened = true;
	bool closed = false;

	bool firstRightClick = true;
	bool firstLeftClick = true;


	Display& display = Display::getInstance();
public:
	bool play_scene = false;
	bool pause_scene = false;
	bool next_frame = false;
	bool scene_started = false;


	void Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity);
};