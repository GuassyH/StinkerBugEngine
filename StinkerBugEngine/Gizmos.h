#ifndef GIZMOS_H
#define GIZMOS_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <xutility>
#include <memory>

#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "EntityHelper.h"
#include "Scene.h"
#include "ECSystem.h"
#include "ComponentsList.h"
#include "Vertex.h"
#include "Constants.h"

#include "EntityBehaviour.h"
#include "ArrowGizmo.h"

namespace Gizmos {

    static inline float gizmo_opacity = 0.95f;
    static inline glm::vec4 x_color = glm::vec4(255.0f / 255.0f, 112.0f / 255.0f, 122.0f / 255.0f, gizmo_opacity);
    static inline glm::vec4 y_color = glm::vec4(142.0f / 255.0f, 255.0f / 255.0f, 142.0f / 255.0f, gizmo_opacity);
    static inline glm::vec4 z_color = glm::vec4(45.0f / 255.0f, 230.0f / 255.0f, 255.0f / 255.0f, gizmo_opacity);
    static inline glm::vec4 origo_color = glm::vec4(0.9f, 0.9f, 0.9f, gizmo_opacity);

    inline Entity& createGizmo(ECSystem& gizmo_registry, const char* name) {
        Entity& entity_id = gizmo_registry.nextEntity;	gizmo_registry.nextEntity++;
        gizmo_registry.component_bits[entity_id] = 0b0;
        gizmo_registry.entity_names[entity_id] = name;
        gizmo_registry.AddComponent<Transform>(entity_id, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0));
        gizmo_registry.AddComponent<GizmoComponent>(entity_id);
        gizmo_registry.entities.insert(entity_id);
        return entity_id;
    }


	struct GizmoObject {
        GizmoObject() = default;
        ~GizmoObject() = default;

        std::shared_ptr<EntityHelper> entity_helper = nullptr;
		std::shared_ptr<MeshRenderer> mr = nullptr;
        std::shared_ptr<GizmoComponent> gc = nullptr;
        std::shared_ptr<EntityBehaviour> eb = nullptr;
		glm::vec3 rotation_offset = glm::vec3(0.0f);
		glm::vec3 position_offset = glm::vec3(0.0f);
		bool needs_neg_z = false;
	};

	struct Gizmo {
        Gizmo() = default;
        ~Gizmo() = default;

        std::vector<GizmoObject> objects;
		void Draw(Camera* camera, Scene& scene, std::shared_ptr<Transform> editor_transform, EntityHelper& selected_entity_helper, bool local_space) // INTERACTABLE
        {
			glm::vec3 cam_to_entity = editor_transform->position + (glm::normalize(selected_entity_helper.GetComponent<Transform>().position - editor_transform->position) * 4.0f);
		    
            for (GizmoObject& obj : objects) {
                if (!obj.mr || !obj.mr->transform || !obj.entity_helper || !obj.gc) { continue; }

                // Set position
			    obj.mr->transform->position = cam_to_entity + obj.position_offset;

                // Set rotation
                if (local_space) {
                    glm::vec3 additional_offset = obj.needs_neg_z ? glm::vec3(0.0f, 0.0f, -selected_entity_helper.GetComponent<Transform>().rotation.z) : glm::vec3(0.0f);
			        obj.mr->transform->rotation = selected_entity_helper.GetComponent<Transform>().rotation + obj.rotation_offset + additional_offset;
                }
                else {
                    obj.mr->transform->rotation = obj.rotation_offset;
                }

                
                // if the the object is hovered change to hovor color.
                if (obj.gc->isHovered && obj.gc->interactable) { obj.mr->material->color = obj.gc->hover_color; }
                else { obj.mr->material->color = obj.gc->reg_color; }

                if (obj.gc->interactable) {
                    if (obj.gc->isHovered && obj.eb) {
                        obj.eb->Update();
                    }
                }
                else {
                    if (obj.eb) {
                        obj.eb->Update();
                    }
                }

                // Render
			    obj.mr->transform->UpdateMatrix();
                obj.mr->model->render(obj.mr->material, obj.mr->transform, camera, nullptr);

                
                // If is hovered reset and set hovered to false
                if (obj.gc->isHovered && obj.gc->interactable) {
                    obj.mr->material->color = obj.gc->reg_color;
                    obj.gc->isHovered = false;
                }
		    }
		}
        void Draw(Camera* camera, Scene& scene, std::shared_ptr<Transform> editor_transform, bool local_space) // NON INTERACTIABLE
        {
            for (GizmoObject& obj : objects) {
                if (!obj.mr || !obj.mr->transform || !obj.entity_helper || !obj.gc) { std::cout << "Gizmo Obj has nullptr" << std::endl; continue; }

                obj.mr->transform->position = editor_transform->position;
                obj.mr->transform->position.y = 0.0;
                
                if (obj.gc->interactable) {
                    if (obj.gc->isHovered && obj.eb) {
                        obj.eb->Update();
                    }
                }
                else {
                    if (obj.eb) {
                        obj.eb->Update();
                    }
                }

                // Render
                obj.mr->transform->UpdateMatrix();
                obj.mr->model->render(obj.mr->material, obj.mr->transform, camera, nullptr);
            }
        }
    }; 

    struct TransformHandle : public Gizmo {
        GizmoObject arrowX;
        GizmoObject arrowY;
        GizmoObject arrowZ;
        GizmoObject translate_origo_point;

        float plane_size = 0.25f;
        float plane_offset = 0.2f;
        float transparency = 0.85f;

        TransformHandle(ECSystem& ecs) {
            arrowZ.needs_neg_z = true;

            // Initialize arrows
            arrowX.entity_helper = std::make_shared<EntityHelper>(createGizmo(ecs, "arrowX"), &ecs);
            arrowY.entity_helper = std::make_shared<EntityHelper>(createGizmo(ecs, "arrowY"), &ecs);
            arrowZ.entity_helper = std::make_shared<EntityHelper>(createGizmo(ecs, "arrowZ"), &ecs);
            translate_origo_point.entity_helper = std::make_shared<EntityHelper>(createGizmo(ecs, "translate_origo_point"), &ecs);

            arrowX.mr = arrowX.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(), std::make_shared<Material>(MaterialFlags_NoDepthTest));
            arrowY.mr = arrowY.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(), std::make_shared<Material>(MaterialFlags_NoDepthTest));
            arrowZ.mr = arrowZ.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(), std::make_shared<Material>(MaterialFlags_NoDepthTest));

            arrowX.eb = arrowX.entity_helper->AddComponentPtr<ArrowGizmo>();
            arrowY.eb = arrowY.entity_helper->AddComponentPtr<ArrowGizmo>();
            arrowZ.eb = arrowZ.entity_helper->AddComponentPtr<ArrowGizmo>();

            arrowX.gc = arrowX.entity_helper->GetComponentPtr<GizmoComponent>();
            arrowY.gc = arrowY.entity_helper->GetComponentPtr<GizmoComponent>();
            arrowZ.gc = arrowZ.entity_helper->GetComponentPtr<GizmoComponent>();

            arrowX.gc->reg_color = x_color;
            arrowY.gc->reg_color = y_color;
            arrowZ.gc->reg_color = z_color;

            // X Arrow
            arrowX.mr->model->loadModel("assets/models/editor/gizmos/arrow/arrow_cubed.gltf");
            arrowX.mr->transform->scale = glm::vec3(0.5f);

            // Y Arrow
            arrowY.mr->model->loadModel("assets/models/editor/gizmos/arrow/arrow_cubed.gltf");
            arrowY.mr->transform->scale = glm::vec3(0.5f);

            // Z Arrow
            arrowZ.mr->model->loadModel("assets/models/editor/gizmos/arrow/arrow_cubed.gltf");
            arrowZ.mr->transform->scale = glm::vec3(0.5f);

            // Ball in the middle 
            translate_origo_point.mr = translate_origo_point.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(Constants::Shapes::Cube()), std::make_shared<Material>(MaterialFlags_NoDepthTest));
            translate_origo_point.gc = translate_origo_point.entity_helper->GetComponentPtr<GizmoComponent>();
            translate_origo_point.mr->material->color.a = transparency;
            translate_origo_point.gc->reg_color = origo_color;
            translate_origo_point.gc->interactable = false;
            translate_origo_point.mr->transform->scale = glm::vec3(0.15f);

            arrowX.rotation_offset = glm::vec3(0.0f, 0.0f, -90.0f);
            arrowY.rotation_offset = glm::vec3(0.0f, 0.0f, 0.0f);
            arrowZ.rotation_offset = glm::vec3(90.0f, 0.0f, 0.0f);

            // Move objects into the vector only after all setup is complete
            objects.emplace_back(arrowX);
            objects.emplace_back(arrowY);
            objects.emplace_back(arrowZ);
            objects.emplace_back(translate_origo_point);
        }
    };

    struct ScaleHandle : public Gizmo {

        GizmoObject scaleX;
        GizmoObject scaleY;
        GizmoObject scaleZ;
        GizmoObject scale_origo_point;

        float plane_size = 0.25f;
        float plane_offset = 0.2f;
        float transparency = 0.85f;

        ScaleHandle(ECSystem& ecs) {
            scaleZ.needs_neg_z = true;

            
            scaleX.entity_helper = std::shared_ptr<EntityHelper>(new EntityHelper(createGizmo(ecs, "scaleX"), &ecs));
            scaleY.entity_helper = std::shared_ptr<EntityHelper>(new EntityHelper(createGizmo(ecs, "scaleY"), &ecs));
            scaleZ.entity_helper = std::shared_ptr<EntityHelper>(new EntityHelper(createGizmo(ecs, "scaleZ"), &ecs));
            scale_origo_point.entity_helper = std::shared_ptr<EntityHelper>(new EntityHelper(createGizmo(ecs, "scale_origo_point"), &ecs));


            scaleX.mr = scaleX.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(), std::make_shared<Material>(MaterialFlags_NoDepthTest));
            scaleY.mr = scaleY.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(), std::make_shared<Material>(MaterialFlags_NoDepthTest));
            scaleZ.mr = scaleZ.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(), std::make_shared<Material>(MaterialFlags_NoDepthTest));

            scaleX.gc = scaleX.entity_helper->GetComponentPtr<GizmoComponent>();
            scaleY.gc = scaleY.entity_helper->GetComponentPtr<GizmoComponent>();
            scaleZ.gc = scaleZ.entity_helper->GetComponentPtr<GizmoComponent>();

            scaleX.gc->reg_color = x_color;
            scaleY.gc->reg_color = y_color;
            scaleZ.gc->reg_color = z_color;


            // X Arrow
            scaleX.mr->model->loadModel("assets/models/editor/gizmos/scale_handle/scale_handle.gltf");
            scaleX.mr->transform->scale = glm::vec3(0.5f);

            // Y Arrow
            scaleY.mr->model->loadModel("assets/models/editor/gizmos/scale_handle/scale_handle.gltf");
            scaleY.mr->transform->scale = glm::vec3(0.5f);

            // Z Arrow
            scaleZ.mr->model->loadModel("assets/models/editor/gizmos/scale_handle/scale_handle.gltf");
            scaleZ.mr->transform->scale = glm::vec3(0.5f);

            // Ball in the middle
            scale_origo_point.mr = scale_origo_point.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(Constants::Shapes::Cube()), std::make_shared<Material>(MaterialFlags_NoDepthTest));
            scale_origo_point.gc = scale_origo_point.entity_helper->GetComponentPtr<GizmoComponent>();
            scale_origo_point.gc->reg_color = origo_color;
            scale_origo_point.gc->interactable = false;
            scale_origo_point.mr->transform->scale = glm::vec3(0.15f);

            scaleX.rotation_offset = glm::vec3(0.0f, 0.0f, -90.0f);
            scaleY.rotation_offset = glm::vec3(0.0f, 0.0f, 0.0f);
            scaleZ.rotation_offset = glm::vec3(90.0f, 0.0f, 0.0f);


            objects.emplace_back(std::move(scaleX));
            objects.emplace_back(std::move(scaleY));
            objects.emplace_back(std::move(scaleZ));
            objects.emplace_back(std::move(scale_origo_point));

        }
	};
    struct RotateHandle : public Gizmo {
        GizmoObject rotateX;
        GizmoObject rotateY;
        GizmoObject rotateZ;

        float plane_size = 0.25f;
        float plane_offset = 0.2f;
        float transparency = 0.85f;

        RotateHandle(ECSystem& ecs) {
            rotateZ.needs_neg_z = true;


            
            rotateX.entity_helper = std::shared_ptr<EntityHelper>(new EntityHelper(createGizmo(ecs, "rotateX"), &ecs));
            rotateY.entity_helper = std::shared_ptr<EntityHelper>(new EntityHelper(createGizmo(ecs, "rotateY"), &ecs));
            rotateZ.entity_helper = std::shared_ptr<EntityHelper>(new EntityHelper(createGizmo(ecs, "rotateZ"), &ecs));

            rotateX.mr = rotateX.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(), std::make_shared<Material>(MaterialFlags_NoDepthTest));
            rotateY.mr = rotateY.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(), std::make_shared<Material>(MaterialFlags_NoDepthTest));
            rotateZ.mr = rotateZ.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(), std::make_shared<Material>(MaterialFlags_NoDepthTest));

            rotateX.gc = rotateX.entity_helper->GetComponentPtr<GizmoComponent>();
            rotateY.gc = rotateY.entity_helper->GetComponentPtr<GizmoComponent>();
            rotateZ.gc = rotateZ.entity_helper->GetComponentPtr<GizmoComponent>();

            rotateX.gc->reg_color = x_color;
            rotateY.gc->reg_color = y_color;
            rotateZ.gc->reg_color = z_color;

            // X Arrow
            rotateX.mr->model->loadModel("assets/models/editor/gizmos/rotation_wheel/rotation_wheel.gltf");
            rotateX.mr->transform->scale = glm::vec3(0.5f);

            // Y Arrow
            rotateY.mr->model->loadModel("assets/models/editor/gizmos/rotation_wheel/rotation_wheel.gltf");
            rotateY.mr->transform->scale = glm::vec3(0.5f);

            // Z Arrow
            rotateZ.mr->model->loadModel("assets/models/editor/gizmos/rotation_wheel/rotation_wheel.gltf");
            rotateZ.mr->transform->scale = glm::vec3(0.5f);

            rotateX.rotation_offset = glm::vec3(0.0f, 0.0f, -90.0f);
            rotateY.rotation_offset = glm::vec3(0.0f, 0.0f, 0.0f);
            rotateZ.rotation_offset = glm::vec3(90.0f, 0.0f, 0.0f);


            objects.emplace_back(std::move(rotateX));
            objects.emplace_back(std::move(rotateY));
            objects.emplace_back(std::move(rotateZ));


        }

    };

    struct InfiniteGrid : public Gizmo {
        GizmoObject infinite_grid;

        InfiniteGrid(ECSystem& ecs) {


            infinite_grid.entity_helper = std::shared_ptr<EntityHelper>(new EntityHelper(createGizmo(ecs, "infinite_grid"), &ecs));

            Shader grid_shader("editor_grid.vert", "editor_grid.frag");
            Material grid_mat(grid_shader);

            infinite_grid.mr = infinite_grid.entity_helper->AddComponentPtr<MeshRenderer>(std::make_shared<Model>(Constants::Shapes::Plane()), std::make_shared<Material>(grid_mat));
            infinite_grid.mr->raycastable = false;
            infinite_grid.gc = infinite_grid.entity_helper->GetComponentPtr<GizmoComponent>();
            infinite_grid.gc->interactable = false;
            infinite_grid.mr->transform->scale = glm::vec3(100.0f, 0.0f, 100.0f);
            infinite_grid.gc->reg_color = glm::vec4(1.0f);
            infinite_grid.gc->hover_color = glm::vec4(1.0f);

            infinite_grid.rotation_offset = glm::vec3(0.0f);
            infinite_grid.position_offset = glm::vec3(0.0f);

            objects.emplace_back(std::move(infinite_grid));

        }
    };
}


#endif
