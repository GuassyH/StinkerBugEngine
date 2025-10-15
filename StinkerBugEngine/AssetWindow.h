#pragma once


#include "Scene.h"
#include "Display.h"
#include "Entity.h"
#include "EntityObject.h"
#include "EditorCamera.h"
#include "Texture.h"
#include "ECSystem.h"

#include "imgui_internal.h"


class AssetWindow {
public:
	AssetWindow() = default;
	void Draw(); // Should not take scene since its scene independant
};
