#include "Window.h"

#include <Events/EventsManager.h>
#include <GLFW/glfw3.h>

Window::Window()
	: resizeDelay(0.2f)
	, size(0)
	, window(nullptr) {}

Window::Window(int const& width, int const& height, const char* title, bool const& fullscreen) : resizeDelay(0.2f) {

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	if (fullscreen) {
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		size = vec2i(mode->width, mode->height);
		window = glfwCreateWindow(mode->width, mode->height, title, nullptr, nullptr);
	} else {
		size = vec2i(width, height);
		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	}

	if (!window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, Window::Resize);

	EventsManager::Get()->Subscribe("CLOSE_WINDOW", &Window::Close, this);
	EventsManager::Get()->Subscribe("_WINDOW_RESIZE", &Window::ResizeHandler, this);
	EventsManager::Get()->Subscribe("BROADCAST_SIZE", &Window::BroadcastSize, this);
	EventsManager::Get()->Subscribe("GET_WINDOW_SIZE", &Window::GetSizeHandler, this);
}

Window::~Window() {
	glfwDestroyWindow(window);
	window = nullptr;
}

GLFWwindow* Window::Get() const {
	return window;
}

void Window::MakeCurrent() const {
	glfwMakeContextCurrent(window);
}

void Window::UpdateFrame(float const& dt) {
	glfwSwapBuffers(window);

	if (resizing) {
		if (bt >= resizeDelay) {
			resizing = false;
			BroadcastSize();
		} else {
			bt += dt;
		}
	}
}

void Window::SetTitle(const char* title) const {
	glfwSetWindowTitle(window, title);
}

void Window::Close() const {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(window);
}

void Window::BroadcastSize() const {
	EventsManager::Get()->Trigger("WINDOW_RESIZE", new Events::AnyType<vec2i>(size));
}

void Window::Resize(GLFWwindow* window, int width, int height) {
	const vec2i size(width, height);
	EventsManager::Get()->Trigger("_WINDOW_RESIZE", new Events::AnyType<vec2i>(size));
}

void Window::ResizeHandler(Events::Event* event) {
	resizing = true;
	bt = 0.0f;
	size = static_cast<Events::AnyType<vec2i>*>(event)->data;
}

void Window::GetSizeHandler(Events::Event* event) const {
	const auto variable = static_cast<Events::AnyType<vec2i*>*>(event)->data;
	*variable = size;
}
