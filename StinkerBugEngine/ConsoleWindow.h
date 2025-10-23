#pragma once

#include "Scene.h"
#include "Display.h"
#include <string.h>
#include "Entity.h"
#include "ConsoleCapture.h"
#include "ECSystem.h"

class ConsoleWindow {
private:
	bool opened = true;
	bool closed = true;

	Display& display = Display::getInstance();
	ConsoleCapture consoleCapture;

public:
	void Draw();
};
