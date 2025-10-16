Stinker Bug Engine is a custom game engine created by me (Gustav Haag) as a fun side project. 
It is made in C++ with OpenGL, glad, and some other libraries. And features an ECS, EntityBehaviour (similar to Unity's MonoBehaviour),
and scene management. As well as an editor UI that lets you easily create scenes and change objects. 

**To create an entity in VS:**
Have an active scene.
Call: EntityObject obj = scene.CreateEntity("Name optional");
To add a component call (T is the component): obj.transform->AddComponent<T>(Args...);
To remove or get call the same but with the respective functions.

**to create an entity in the Engine:**
Have an active scene.
Right click on the hierarchy inspector.
Click on "create Empty Object" or any preset.
To add a component click on the object you want to add to, and in the inspector window click Add Component.

**Custom Components**

Creating your own component is simple. Simply create a header file in VS (i would recommend in the components folder) and follow this format.

#ifndef COMPONENTNAME_COMPONENT_H
#define COMPONENTNAME_COMPONENT_H

#include "Component.h"

class ComponentName : public Component{
public:
  virtual void Init() override {
    // Any initialisation you want OPTIONAL
  }
  virtual void DrawOnInspector() override {
    // ImGui code OPTIONAL
  }
}

#endif

**Models**

Loading models is easy. Simply create an entity then follow this step-by-step:
new_entity.transform->AddComponent<MeshRenderer>(std::make_shared<Model>(), std::make_shared<Material>(MaterialFlags_Lit | MaterialFlags_Shadow | MaterialFlags_Depth));
new_entity.transform->GetComponent<MeshRenderer>.model->loadModel("assets/models/file_path/file_scene.gltf");
new_entity.transform->GetComponent<MeshRenderer>.material->Color = Constants::Colors::White *OR* glm:vec4(1.0f);

*NOTE* 
You need to set the path of the model to the *scene.gltf* not just the folder. The model loader will manage the rest

**EntityBehaviour**
The EntityBehaviour class is what allows custom scripts that can be added to the object.
Its easy, just create a header file with the class name. Inherit the class from EntityBehaviour. And add the virtual void override functions.
This might be changed to be in C# using scripting layers.

**UI**
Additionally, if you want to change the gizmo selected press W (transform handle), S (scale handle), R (rotation handle)

**DISCLAIMER**
This is more of a passion project, so it might not get regular updates, and it will be very scuffed.
