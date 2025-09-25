#pragma once

#include "Scene.h"
#include "Display.h"
#include "Entity.h"
#include "EntityHelper.h"
#include "EditorCamera.h"
#include "Texture.h"

class SceneViewWindow {
private:
	bool opened = true;
	bool closed = false;

	bool firstRightClick = true;
	bool firstLeftClick = true;


	Display& display = Display::getInstance();
public:

	EditorCamera* editorCamera = nullptr;
	Texture* cam_output = nullptr;

	SceneViewWindow() = default;
	void Init();
	void Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity);
	
};