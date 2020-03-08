#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"

#include <Events/Event.h>

#include <stack>
#include <queue>

class SceneManager {

	std::stack<Scene*> sceneStack;
	std::queue<Scene*> queuedScenes;

public:

	SceneManager();
	~SceneManager();

	Scene* GetSource() const;
	void Queue(Scene * const scene);
	void Present(Scene * const scene);
	void Segue();

private:

	void PresentHandler(Events::Event* event);
	void DismissHandler();

};

#endif
