#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"

#include <Events/Event.h>

#include <map>
#include <string>

class SceneManager {

	std::string source;
	std::string destination;

	std::map<std::string, Scene*> scenes;

public:

	SceneManager();
	~SceneManager();

	void Add(const std::string& identifier, Scene * const scene);
	Scene* GetSource();

	void SetEntryPoint(const std::string& identifier);
	void Segue();

private:

	void PresentHandler(Events::Event* event);

};

#endif
