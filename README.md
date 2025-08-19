Stinker Bug Game Engine made by me. This engine has an ECS that I made with the help of some studying. Learning a lot about Templates and std::enable_if_t.
Feel free to try to make something with this but it will probably be incredibly difficult haha.


to create an entity:
Create a SceneManager and then create a Scene scene. 
Then write: Entity your_entity_name = scene.CreateEntity();
Then you can: your_entity_name.AddComponent<Component>();

As of how the ECS works now (19/08/2025) if you want to add a component, first write the component struct/class in a header file. Go to ComponenetsList and add #include "YourComponent.h", then go to the ECS file and add an unordered_map<uint32_t, YourComponent> your_components; And lastly create a new template for GetComponentMap (should be self explanatory).
Then you can add it and get it etc.

I might make an actual tutorial doc later
