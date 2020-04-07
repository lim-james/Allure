#ifndef WINDOW_H
#define WINDOW_H

#include <Math/Vectors.h>
#include <Events/Event.h>

struct GLFWwindow;

class Window {

	const float resizeDelay;
	bool resizing;

	float bt;

	vec2i size;

	GLFWwindow* window;

public:

	Window();
	Window(int const& width, int const& height, const char* title, bool const& fullscreen);
	~Window();

	GLFWwindow* Get() const;

	void MakeCurrent() const;
	void UpdateFrame(float const& dt);

	void SetTitle(const char* title) const;

	void Close() const;
	bool ShouldClose() const;

	void BroadcastSize() const;

private:

	static void Resize(GLFWwindow* window, int width, int height);

	void ResizeHandler(Events::Event* event);
	void GetSizeHandler(Events::Event* event) const;

};

#endif
