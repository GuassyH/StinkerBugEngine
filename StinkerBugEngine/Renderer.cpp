#include "Renderer.h"

#include "Component.h"
#include "MeshRenderer.h"
#include "GizmoComponent.h"
#include "Transform.h"

void Renderer::rebuildMeshLists(std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<Component>>>& components) {
    clearMeshes();

    queue_mesh_rebuild = false;
    
    auto& meshMap = components[std::type_index(typeid(MeshRenderer))];
    auto& transformMap = components[std::type_index(typeid(Transform))];

    for (auto& [id, compPtr] : meshMap) {
        if (!compPtr) continue;
        if (components[std::type_index(typeid(GizmoComponent))].find(id) != components[std::type_index(typeid(GizmoComponent))].end()) { continue; }

        auto rendererPtr = std::static_pointer_cast<MeshRenderer>(compPtr);
        if (!rendererPtr->model || !rendererPtr->material) continue;

        auto itT = transformMap.find(id);
        if (itT == transformMap.end() || !itT->second) continue;
        auto transformPtr = std::static_pointer_cast<Transform>(itT->second);

        // Could perform frustum culling here

        ObjectCall new_call{ std::weak_ptr<MeshRenderer>(rendererPtr), std::weak_ptr<Transform>(transformPtr) };

        if (rendererPtr->material->HasFlag(MaterialFlags_Transparent)) {
            transparent_meshes.push_back(new_call);
        }
        else {
            opaque_meshes.push_back(new_call);
        }
    }
}

void Renderer::sortTransparentMeshes(glm::vec3& origo) {
    std::sort(transparent_meshes.begin(), transparent_meshes.end(), [&origo](const ObjectCall& a, const ObjectCall& b) {
        float da = glm::length2(origo - a.transform->position);
        float db = glm::length2(origo - b.transform->position);
        return da > db;
        });
}




void Renderer::recalculateLightsBuffer(std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<Component>>>& components) {
    auto& lightMap = components[std::type_index(typeid(Light))];

    std::vector<LightBufferObject> light_objs;

    numLights = 0;
    for (auto& [id, l] : lightMap) {
        if (!lightMap.contains(id)) { continue; }

        auto it = lightMap.find(id);
        Light& light = *std::static_pointer_cast<Light>(it->second);
        light.transform->UpdateMatrix();

        if (light.light_type == LightTypes::Directional) { continue; }

        LightBufferObject new_lbo;

        new_lbo.type = int(light.light_type);
        new_lbo.intensity = light.intensity;
        new_lbo.radius = light.radius;
        new_lbo.radius_i = light.radius_i;
        new_lbo.radius_o = light.radius_o;
        new_lbo.pos = light.transform->position;
        new_lbo.dir = light.transform->DegToVec();
        new_lbo.color = light.color;

        numLights++;
        light_objs.push_back(new_lbo);
    }

    lightBuffSize = sizeof(LightBufferObject) * numLights;


    if (lightBufferID) glDeleteBuffers(1, &lightBufferID);
    glGenBuffers(1, &lightBufferID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, lightBufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, lightBuffSize, light_objs.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightBufferID);
    queue_lights_rebuild = false;

}

void Renderer::bindLightsBuffer() {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, lightBufferID);
}

void Renderer::deleteLightsBuffer() {
    glDeleteBuffers(1, &lightBufferID);
}