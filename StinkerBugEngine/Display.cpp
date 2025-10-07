#include "Display.h"
#include "Texture.h"

double currentTime;
double lastTime;
int nbFrames;

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	Display::getInstance().windowWidth = width;
	Display::getInstance().windowHeight = height;
	std::cout << "window size is " << width << " x " << height << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	// Temporary
	Display::getInstance().scroll = yoffset;
}

int Display::Init(unsigned int windowWidth, unsigned int windowHeight, const char* title){
	Display::windowWidth = windowWidth;
	Display::windowHeight = windowHeight;
	Display::title = title;

	if (!glfwInit()) { return -1; }

	// Set opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, title, NULL, NULL);
	monitor = glfwGetWindowMonitor(window);

	if (!window) {
		std::cout << "Failed to create Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // Turn off v-sync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;

	// Get monitor height
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	monitorWidth = mode->width;
	monitorHeight = mode->height;

	// Set Frame Buffer Size Callback
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Culling stuff
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Display::SetSize(windowWidth, windowHeight);
	
	// Icon!!
	GLFWimage icon_image = GLFWimage();
	int icon_size_x, icon_size_y, numColCh;
	icon_image.pixels = stbi_load("assets/textures/editor/StinkerBugIcon.png", &icon_size_x, &icon_size_y, &numColCh, 4);
	icon_image.height = icon_size_y;
	icon_image.width = icon_size_x;

	glfwSetWindowIcon(window, 1, &icon_image);


	std::cout << "Display / GLFW initialized\n";

	return 0;
}

void Display::BeginFrame() {
	DeltaTime::getInstance().update();
	currentTime = glfwGetTime();
	nbFrames++;

	glClearColor(0.1f, 0.1f, 0.13f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glViewport(0, 0, windowWidth, windowHeight); // Should i do this?
}

void Display::EndFrame() {
	glViewport(0, 0, windowWidth, windowHeight); // Should i do this?
	if (currentTime - lastTime >= 1.0) {
		FrameRate = nbFrames;
		glfwSetWindowTitle(window, (std::string(title) + " - " + std::to_string(FrameRate) + " FPS").c_str());
		nbFrames = 0;
		lastTime += 1.0;
	}

	scroll = 0.0f;

	if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void Display::SetSize(unsigned int width, unsigned int height) {
	glfwSetWindowSize(window, windowWidth, windowHeight);
	glfwSetWindowPos(window, (monitorWidth - windowWidth) / 2, (monitorHeight - windowHeight) / 2);

	std::cout << "New Window Size : " << width << "px * " << height << "px\n\n";
}

Display::~Display() {
	std::cout << "Display Shutdown\n";
	glfwDestroyWindow(window);
	glfwTerminate();
}



