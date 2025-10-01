#pragma once

#include "Scene.h"
#include "Display.h"
#include <string.h>
#include "Entity.h"
#include "ConsoleCapture.h"

class ConsoleWindow {
private:
	bool opened = true;
	bool closed = true;

	Display& display = Display::getInstance();
	ConsoleCapture consoleCapture;

public:
	void Draw(Scene& scene, bool& is_entity_selected, Entity& selected_entity);
};
