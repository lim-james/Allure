#ifndef BASE_SCRIPT_H
#define BASE_SCRIPT_H

#include "Component.h"

struct BaseScript : Component {

	// when script is created
	void Initialize() override;

	// when the script becomes active
	virtual void Awake();
	// when the scene enters
	virtual void Start();
	// every fixed timestep
	virtual void FixedUpdate();
	// every frame
	virtual void Update();
	// when the scene exits
	virtual void Stop();
	// when the script becomes inactive or scene is destroyed
	virtual void OnDestroy();

	void SetActive(const bool& state) override;

};

#endif
