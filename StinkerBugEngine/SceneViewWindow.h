#pragma once

#include "Scene.h"
#include "Display.h"
#include "Entity.h"
#include "EntityObject.h"
#include "EditorCamera.h"
#include "Texture.h"
#include "ECSystem.h"

#include "imgui_internal.h"

class SceneViewWindow {
private:
	bool opened = true;
	bool closed = false;

	bool firstRightClick = true;
	bool firstLeftClick = true;

	bool showStats = false;

	Display& display = Display::getInstance();
public:

	EditorCamera* editorCamera = nullptr;
	Texture* cam_output = nullptr;

	SceneViewWindow() = default;
	void Init(ECSystem& editor_ecs);
	void Draw(Scene* scene, bool& is_entity_selected, Entity& selected_entity, ECSystem& editor_ecs);
	
};