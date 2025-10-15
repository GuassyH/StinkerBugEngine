#include "ECSystem.h"
#include "MeshRenderer.h"
#include "GizmoComponent.h"

void ECSystem::DestroyEntity(Entity& id) {
	bool update_renderer = false;
	if (GetComponentMap<MeshRenderer>().find(id) != GetComponentMap<MeshRenderer>().end()) {
		if (components[std::type_index(typeid(GizmoComponent))].find(id) == components[std::type_index(typeid(GizmoComponent))].end()) {
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
		Renderer::getInstance().rebuildMeshLists(components);
	}
}

void ECSystem::DuplicateEntity(Entity& id) {

}