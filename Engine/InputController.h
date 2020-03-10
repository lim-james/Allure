#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <Math/Vectors.h>
#include <Events/Event.h>

struct GLFWwindow;

class InputController {

	GLFWwindow* context;

	float sensitivity;
	vec2f cursorPosition;

public:

	InputController();

	void Initialize(GLFWwindow* current);

private:

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CharCallback(GLFWwindow* window, unsigned codepoint);
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int key, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void DropCallback(GLFWwindow* window, int count, const char** paths);

	void OnEvent(Events::Event* event);

};

#endif

