#ifndef BUTTON_SYSTEM_H
#define BUTTON_SYSTEM_H

#include "System.h"
#include "Button.h"
#include "Camera.h"

#include <Events/Event.h>
#include <Math/Vectors.h>

#include <vector>
#include <map>

class ButtonSystem : public System {

	std::map<Button*, bool> states;
	std::vector<Button*> buttons;
	std::vector<Camera*> cameras;

	vec2f windowSize;
	vec2f mousePosition;
	std::map<int, int> prevMouseActions;
	std::map<int, int> mouseActions;

public:

	~ButtonSystem() override;

	void Initialize() override;
	void Update(const float& dt) override;

private:

	void CameraActiveHandler(Events::Event* event);
	void ButtonActiveHandler(Events::Event* event);

	void ResizeHandler(Events::Event* event);

	void CursorPositionHandler(Events::Event* event);
	void MouseButtonHandler(Events::Event* event);

	void PerformAction(const unsigned& index, Button* const self);

};

#endif
