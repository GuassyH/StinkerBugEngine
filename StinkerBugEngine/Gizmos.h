#ifndef GIZMOS_H
#define GIZMOS_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "EntityHelper.h"
#include "Scene.h"
#include "ComponentsList.h"
#include "Vertex.h"
#include "Constants.h"

namespace Gizmos {
	struct GizmoObject {
		MeshRenderer mesh_renderer;
		Transform transform;
		glm::vec3 rotation_offset = glm::vec3(0.0f);
		glm::vec3 position_offset = glm::vec3(0.0f);
		bool needs_neg_z = false;
	};

	struct Gizmo {
		public:
			std::vector<GizmoObject> objects;
			void Draw(Camera* camera, Scene& scene, Transform* editor_transform, EntityHelper& selected_entity_helper) {
			for (GizmoObject obj : objects) {
				glm::vec3 cam_to_entity = editor_transform->position + (glm::normalize(selected_entity_helper.GetComponent<Transform>().position - editor_transform->position) * 3.0f);
				obj.transform.position = cam_to_entity + obj.position_offset;

				glm::vec3 additional_offset = obj.needs_neg_z ? glm::vec3(0.0f, 0.0f, -selected_entity_helper.GetComponent<Transform>().rotation.z) : glm::vec3(0.0f);
				obj.transform.rotation = selected_entity_helper.GetComponent<Transform>().rotation + obj.rotation_offset + additional_offset;

				obj.transform.UpdateMatrix();
                if (scene.HasMainLight()) {
				    obj.mesh_renderer.model->render(obj.mesh_renderer.material, &obj.transform, editor_transform, camera, &scene.main_light->GetComponent<Light>());
                }
                else {
                    obj.mesh_renderer.model->render(obj.mesh_renderer.material, &obj.transform, editor_transform, camera, nullptr);
                }

			}
		}
	};

	struct TransformHandle : public Gizmo {
		GizmoObject arrowX;
		GizmoObject arrowY;
		GizmoObject arrowZ;
		GizmoObject origo_point;

        float plane_size = 0.25f;
        float plane_offset = 0.2f;
        float transparency = 0.85f;

        TransformHandle() {
            arrowZ.needs_neg_z = true;
            // Initialize arrows
            arrowX.mesh_renderer = MeshRenderer();
            arrowY.mesh_renderer = MeshRenderer();
            arrowZ.mesh_renderer = MeshRenderer();
            origo_point.mesh_renderer = MeshRenderer();

            arrowX.transform = Transform();
            arrowY.transform = Transform();
            arrowZ.transform = Transform();
            origo_point.transform = Transform();

            // X Arrow
            arrowX.mesh_renderer.model = new Model();
            arrowX.mesh_renderer.model->loadModel("assets/models/gizmos/arrow/arrow.gltf");
            arrowX.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            arrowX.mesh_renderer.material->Color = Constants::Colors::Red;
            arrowX.mesh_renderer.material->Color.a = transparency;
            arrowX.transform.scale = glm::vec3(0.5f);

            // Y Arrow
            arrowY.mesh_renderer.model = new Model();
            arrowY.mesh_renderer.model->loadModel("assets/models/gizmos/arrow/arrow.gltf");
            arrowY.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            arrowY.mesh_renderer.material->Color = Constants::Colors::Green;
            arrowY.mesh_renderer.material->Color.a = transparency;
            arrowY.transform.scale = glm::vec3(0.5f);

            // Z Arrow
            arrowZ.mesh_renderer.model = new Model();
            arrowZ.mesh_renderer.model->loadModel("assets/models/gizmos/arrow/arrow.gltf");
            arrowZ.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            arrowZ.mesh_renderer.material->Color = Constants::Colors::Blue;
            arrowZ.mesh_renderer.material->Color.a = transparency;
            arrowZ.transform.scale = glm::vec3(0.5f);

            // Ball in the middle
            Mesh origo_mesh = Mesh(Constants::Shapes::Cube());
            origo_point.mesh_renderer.model = new Model(origo_mesh);
            origo_point.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            origo_point.mesh_renderer.material->Color = Constants::Colors::White;
            origo_point.mesh_renderer.material->Color.a = transparency;
            origo_point.transform.scale = glm::vec3(0.1f);

			arrowX.rotation_offset = glm::vec3(0.0f, 0.0f, -90.0f);
			arrowY.rotation_offset = glm::vec3(0.0f, 0.0f, 0.0f);
			arrowZ.rotation_offset = glm::vec3(90.0f, 0.0f, 0.0f);

			objects.push_back(arrowX);
			objects.push_back(arrowY);
			objects.push_back(arrowZ);

			objects.push_back(origo_point);
        }
	};
    struct ScaleHandle : public Gizmo {
        GizmoObject arrowX;
        GizmoObject arrowY;
        GizmoObject arrowZ;

        GizmoObject origo_point;

        GizmoObject XY_plane;
        GizmoObject ZY_plane;
        GizmoObject XZ_plane;

		float plane_size = 0.25f;
		float plane_offset = 0.2f;
		float transparency = 0.85f;

        ScaleHandle() {
            arrowZ.needs_neg_z = true;

            // Initialize scale handles
            arrowX.mesh_renderer = MeshRenderer();
            arrowY.mesh_renderer = MeshRenderer();
            arrowZ.mesh_renderer = MeshRenderer();
            origo_point.mesh_renderer = MeshRenderer();

			XY_plane.mesh_renderer = MeshRenderer();
			ZY_plane.mesh_renderer = MeshRenderer();
			XZ_plane.mesh_renderer = MeshRenderer();

            arrowX.transform = Transform();
            arrowY.transform = Transform();
            arrowZ.transform = Transform();
            origo_point.transform = Transform();

			XY_plane.transform = Transform();
			ZY_plane.transform = Transform();
			XZ_plane.transform = Transform();

            // X Arrow
            arrowX.mesh_renderer.model = new Model();
            arrowX.mesh_renderer.model->loadModel("assets/models/gizmos/scale_handle/scale_handle.gltf");
            arrowX.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            arrowX.mesh_renderer.material->Color = Constants::Colors::Red;
            arrowX.mesh_renderer.material->Color.a = transparency;
            arrowX.transform.scale = glm::vec3(0.5f);

            // Y Arrow
            arrowY.mesh_renderer.model = new Model();
            arrowY.mesh_renderer.model->loadModel("assets/models/gizmos/scale_handle/scale_handle.gltf");
            arrowY.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            arrowY.mesh_renderer.material->Color = Constants::Colors::Green;
            arrowY.mesh_renderer.material->Color.a = transparency;
            arrowY.transform.scale = glm::vec3(0.5f);

            // Z Arrow
            arrowZ.mesh_renderer.model = new Model();
            arrowZ.mesh_renderer.model->loadModel("assets/models/gizmos/scale_handle/scale_handle.gltf");
            arrowZ.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            arrowZ.mesh_renderer.material->Color = Constants::Colors::Blue;
            arrowZ.mesh_renderer.material->Color.a = transparency;
            arrowZ.transform.scale = glm::vec3(0.5f);

            // Ball in the middle
            Mesh origo_mesh = Mesh(Constants::Shapes::Cube());
            origo_point.mesh_renderer.model = new Model(origo_mesh);
            origo_point.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            origo_point.mesh_renderer.material->Color = Constants::Colors::White;
            origo_point.mesh_renderer.material->Color.a = transparency;
            origo_point.transform.scale = glm::vec3(0.1f);

            arrowX.rotation_offset = glm::vec3(0.0f, 0.0f, -90.0f);
            arrowY.rotation_offset = glm::vec3(0.0f, 0.0f, 0.0f);
            arrowZ.rotation_offset = glm::vec3(90.0f, 0.0f, 0.0f);

			// XY Plane THEY NEED TO PIVOT AROUND THE AXIS
			Mesh plane_mesh = Mesh(Constants::Shapes::Plane());
			XY_plane.mesh_renderer.model = new Model(plane_mesh);
			XY_plane.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
			XY_plane.mesh_renderer.material->Color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
            XY_plane.mesh_renderer.material->Color.a = transparency;
            XY_plane.transform.scale = glm::vec3(plane_size);
			XY_plane.position_offset = glm::vec3(plane_offset, plane_offset, 0.0f);
			XY_plane.rotation_offset = glm::vec3(90.0f, 0.0f, 0.0f);

            XZ_plane.mesh_renderer.model = new Model(plane_mesh);
            XZ_plane.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            XZ_plane.mesh_renderer.material->Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
            XZ_plane.mesh_renderer.material->Color.a = transparency;
            XZ_plane.transform.scale = glm::vec3(plane_size);
            XZ_plane.position_offset = glm::vec3(plane_offset, 0.0f, plane_offset);
            XZ_plane.rotation_offset = glm::vec3(0.0f, 0.0f, 0.0f);
            
            ZY_plane.mesh_renderer.model = new Model(plane_mesh);
            ZY_plane.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            ZY_plane.mesh_renderer.material->Color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
            ZY_plane.mesh_renderer.material->Color.a = transparency;
            ZY_plane.transform.scale = glm::vec3(plane_size);
            ZY_plane.position_offset = glm::vec3(0.0f, plane_offset, plane_offset);
            ZY_plane.rotation_offset = glm::vec3(0.0f, 0.0f, -90.0f);


            objects.push_back(arrowX);
            objects.push_back(arrowY);
            objects.push_back(arrowZ);
            // objects.push_back(XY_plane);
            // objects.push_back(XZ_plane);
			// objects.push_back(ZY_plane);

            objects.push_back(origo_point);
        }
	};
    struct RotateHandle : public Gizmo {
        GizmoObject circleX;
        GizmoObject circleY;
        GizmoObject circleZ;


        float transparency = 0.85f;
    
        RotateHandle() {
            // Initialize arrows
            circleX.mesh_renderer = MeshRenderer();
            circleY.mesh_renderer = MeshRenderer();
            circleZ.mesh_renderer = MeshRenderer();

            circleX.transform = Transform();
            circleY.transform = Transform();
            circleZ.transform = Transform();
            circleZ.needs_neg_z = true;

            // X Arrow
            circleX.mesh_renderer.model = new Model();
            circleX.mesh_renderer.model->loadModel("assets/models/gizmos/rotation_wheel/rotation_wheel.gltf");
            circleX.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            circleX.mesh_renderer.material->Color = Constants::Colors::Red;
            circleX.mesh_renderer.material->Color.a = transparency;
            circleX.transform.scale = glm::vec3(0.51f);

            // Y Arrow
            circleY.mesh_renderer.model = new Model();
            circleY.mesh_renderer.model->loadModel("assets/models/gizmos/rotation_wheel/rotation_wheel.gltf");
            circleY.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            circleY.mesh_renderer.material->Color = Constants::Colors::Green;
            circleY.mesh_renderer.material->Color.a = transparency;
            circleY.transform.scale = glm::vec3(0.5f);

            // Z Arrow
            circleZ.mesh_renderer.model = new Model();
            circleZ.mesh_renderer.model->loadModel("assets/models/gizmos/rotation_wheel/rotation_wheel.gltf");
            circleZ.mesh_renderer.material = new Material(MaterialFlags_NoDepthTest);
            circleZ.mesh_renderer.material->Color = Constants::Colors::Blue;
            circleZ.mesh_renderer.material->Color.a = transparency;
            circleZ.transform.scale = glm::vec3(0.49f);


            circleX.rotation_offset = glm::vec3(0.0f, 0.0f, -90.0f);
            circleY.rotation_offset = glm::vec3(0.0f, 0.0f, 0.0f);
            circleZ.rotation_offset = glm::vec3(90.0f, 0.0f, 0.0f);

            objects.push_back(circleX);
            objects.push_back(circleY);
            objects.push_back(circleZ);
        }

    };
}


#endif
