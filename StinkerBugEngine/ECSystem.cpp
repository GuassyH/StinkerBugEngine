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
		Renderer::getInstance().queue_mesh_rebuild = true;
	}
}

void ECSystem::DuplicateEntity(Entity& id) {
	Entity& new_id = nextEntity;	nextEntity++;
	component_bits[new_id] = 0b0;

	AddComponent<Transform>(new_id, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0));
	
	int tries = 0;
	std::string name = entity_names[id];
	std::string new_name = name;

	// Go through each variation of the name until a new one is found
	while (std::any_of(entity_names.begin(), entity_names.end(),
		[&](const auto& pair) {
			return pair.second == new_name;
		})) {
		tries++;
		new_name = name + " (" + std::to_string(tries) + ")";
	}

	if (tries > 0) {
		name = new_name;
	}

	entity_names[new_id] = name;

	for (auto& [type, map] : components) {
		auto compPtr = map.find(id);
		if (compPtr != map.end() && compPtr->second && type != typeid(Transform)) {
			// Add
		}
	}
	
	entities.insert(new_id);
}