Stinker Bug Game Engine made by me. This engine has an ECS that I made with the help of some studying. Learning a lot about Templates, bitwise, ECS, and more.
Feel free to try to make something with this but it will probably be incredibly difficult haha.


to create an entity in VS:
Create a SceneManager and then create a Scene scene. 
Then go Entity& entity = scene.CreateEntity(); // If you want to name it put "x" in parentheses
Then to add / remove / get components you must create an EntityHelper, which contains these function. To do that go
EntityHelper e_helper(entity, scene.Scene_ECS);
then call e_helper.AddComponent<T>(Args...);

to create an entity in the Engine:
have a scene loaded
Right click on the hierarchy and press "Create Empty Object" or another option.
Click on the object (or anyone you want to change) and the components will appear on the Inspector Window.
To AddComponent press the AddComponent button in the bottom of the inspector

If you want to create your own component it is simple:
Create the Header File, say TestComponent.h for example.
Then the component NEEDS to include Component.h and derive from it.
**NOTE** the component can be struct or class. And to create a GUI window for it you need to say:
virtual void DrawOnInspector() override{
  if(ImGui::CollapsingHeader("Name")){
    // Whatever you want to display here
  }
}

Loading models is easy. Simply create an entity then follow this step-by-step:
new_entity.AddComponent<MeshRenderer>(new Model(), new Material(MaterialFlags_Lit | MaterialFlags_Shadow | MaterialFlags_Depth);
new_entity.GetComponent<MeshRenderer>.model->loadModel("assets/models/file_path/file_scene.gltf");
new_entity.GetComponent<MeshRenderer>.material->Color = Constants::Colors::White *OR* glm:vec4(1.0f);

*NOTE* 
You need to set the path to the *scene* not just the folder. The model loader will manage the rest

Additionally, if you want to change the gizmo selected press W (transform handle), S (scale handle), R (rotation handle)

* DISCLAIMER *
This is more of a passion project, so it might not get regular updates, and it will be very scuffed.
