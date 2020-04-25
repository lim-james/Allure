#ifndef LLAMA_SCENE_H
#define LLAMA_SCENE_H

#include "Scene.h"

class VoxelScene : public Scene {
	
public:

	void Awake() override;
	void Create() override;

private:

	void CreateEditor();

};

#endif
