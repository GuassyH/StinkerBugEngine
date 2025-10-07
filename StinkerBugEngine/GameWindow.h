#pragma once

#include "Scene.h"
#include "Display.h"
#include "Entity.h"
#include "EntityHelper.h"
#include "EditorCamera.h"
#include "Texture.h"
#include "ECSystem.h"
#include "imgui_internal.h"

class GameWindow {
private:
	bool opened = true;
	bool closed = false;

	Display& display = Display::getInstance();
public:
	void Init();
	void Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs);
};