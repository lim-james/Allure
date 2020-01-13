#ifndef COMPONENT_H
#define COMPONENT_H

struct Component {

	unsigned entity;

	Component();
	virtual ~Component();

	virtual void Initialize() = 0;

	const bool& IsActive() const;
	virtual void SetActive(const bool& _active);

private:

	bool active;

};

#endif
