#ifndef COMPONENT_H
#define COMPONENT_H

#define ACTIVE_BIT	0b1
#define DYNAMIC_BIT	0b10

struct Component {

	unsigned entity;

	Component();
	virtual ~Component();

	virtual void Initialize() = 0;
	virtual Component* Clone() const = 0;

	bool IsActive() const;
	virtual void SetActive(const bool& _active);

	bool IsDynamic() const;
	virtual void SetDynamic(const bool& _active);

private:

	unsigned flags;

};

#endif
