#ifndef SCRIPT_H
#define SCRIPT_H

#include "Component.h"

#include "Time.h"
#include "EntityManager.h"
#include "Transform.h"

struct Script : Component {

	Time * time;

	EntityManager * entities;
	Transform * transform;

	// when script is created
	void Initialize() override;
	void SetActive(const bool& state) override;

protected:

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

	template<typename ComponentType>
	ComponentType * GetComponent();

	friend class ScriptSystem;

};

template<typename ComponentType>
ComponentType * Script::GetComponent() {
	return entities->GetComponent<ComponentType>(entity);
}

#endif
