#ifndef ENTITY_SELECTOR_H
#define ENTITY_SELECTOR_H

#include "Scene.h"
#include "Display.h"
#include "Entity.h"

class EntitySelector {
public:
	void Draw(Scene& scene, Entity& ID, bool& is_entity_selected, Entity& selected_entity);
};

#endif