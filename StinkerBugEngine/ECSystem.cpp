#include "ECSystem.h"
#include "MeshRenderer.h"
#include "GizmoComponent.h"

void ECSystem::DestroyEntity(Entity& id) {
	bool update_renderer = false;
	if (GetComponentMap<MeshRenderer>().contains(id)) {
		if (!GetComponentMap<GizmoComponent>().contains(id)) {
			update_renderer = true;
		}
	}

	// remove all tracked things
	entities.erase(id);
	entity_names.erase(id);
	colliders.erase(id);
	entity_behaviours.erase(id);
	component_bits.erase(id);

	// erase from component maps
	for (auto& [typeIdx, map] : components) {
		map.erase(id);
	}

	if (update_renderer) {
		Renderer::getInstance().queue_rebuild = true;
	}
}

void ECSystem::DuplicateEntity(Entity& id) {
	Entity& entity_id = nextEntity;	nextEntity++;
	component_bits[entity_id] = 0b0;

	entity_names[entity_id] = entity_names.find(id)->second + " (" + std::to_string(entity_id) + ")";
	AddComponent<Transform>(entity_id, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0));
	
	for (auto& [type, map] : components) {
		auto compPtr = map.find(id);
		if (compPtr != map.end() && compPtr->second && type != typeid(Transform)) {
			// Should add component
		}
	}
	
	entities.insert(entity_id);
}