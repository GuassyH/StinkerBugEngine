#pragma once

#include "Scene.h"
#include "Display.h"
#include "Entity.h"
#include "ECSystem.h"
#include "Texture.h"

class TopBarWindow {
private:
	bool opened = true;
	bool closed = false;

	bool firstRightClick = true;
	bool firstLeftClick = true;

	Texture PlayTexture;
	Texture PauseTexture;
	Texture NextTexture;

	Texture PlayingBlip;
	Texture PausedBlip;

	Display& display = Display::getInstance();
public:
	bool play_scene = false;
	bool pause_scene = false;
	bool next_frame = false;
	bool scene_started = false;

	void Init();
	void Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs);
};