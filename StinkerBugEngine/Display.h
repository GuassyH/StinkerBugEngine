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
	int windowWidth = 1920;
	int windowHeight = 1080;

	int monitorWidth = 1;
	int monitorHeight = 1;

	const char* title = "New Display";

	GLFWwindow* window = nullptr;
	GLFWmonitor* monitor = nullptr;
	
	static Display& getInstance() { static Display instance; return instance; }

	Display(const Display&) = delete;
	Display& operator=(const Display&) = delete;

	int Init(unsigned int windowWidth, unsigned int windowHeight, const char* title);
	void BeginFrame();
	void EndFrame();
	void SetSize(unsigned int width, unsigned int height);
	~Display();

	int FrameRate;
	double scroll = 0.0;
};



#endif