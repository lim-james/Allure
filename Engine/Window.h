#ifndef WINDOW_H
#define WINDOW_H

#include <Math/Vectors.h>
#include <Events/Event.h>

struct GLFWwindow;

class Window {

	vec2i size;

	GLFWwindow* window;

public:

	Window();
	Window(const int& width, const int& height, const char* title, const bool& fullscreen);
	~Window();

	GLFWwindow* Get() const;

	void MakeCurrent() const;

	void SetTitle(const char* title) const;

	void Close() const;
	bool ShouldClose() const;
	void SwapBuffers() const;

	void BroadcastSize() const;

private:

	static void Resize(GLFWwindow* window, int width, int height);

	void ResizeHandler(Events::Event* event);
	void GetSizeHandler(Events::Event* event) const;

};

#endif
