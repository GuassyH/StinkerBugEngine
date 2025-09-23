#ifndef DISPLAY_H
#define DISPLAY_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

#include "DeltaTime.h"

class Display {
private:
	Display() = default;
public:
	int windowWidth;
	int windowHeight;


	int monitorWidth;
	int monitorHeight;

	const char* title;

	GLFWwindow* window;
	GLFWmonitor* monitor;
	
	static Display& getInstance() { static Display instance; return instance; }

	Display(const Display&) = delete;
	Display& operator=(const Display&) = delete;

	int init(unsigned int windowWidth, unsigned int windowHeight, const char* title);
	void BeginFrame();
	void EndFrame();
	void SetSize(unsigned int width, unsigned int height);
	~Display();

	int FrameRate;
};



#endif