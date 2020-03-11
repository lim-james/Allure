#ifndef COMPONENT_H
#define COMPONENT_H

#include <Encoding/AEObject.h>

#define ACTIVE_BIT	0b1
#define DYNAMIC_BIT	0b10

struct Component : AEObject {

	unsigned entity;

	Component();
	virtual ~Component();

	virtual void Initialize() = 0;
	virtual Component* Clone() const = 0;

	bool IsActive() const;
	virtual void SetActive(const bool& state);

	bool IsDynamic() const;
	virtual void SetDynamic(const bool& state);

	virtual void Unpack(const AENotation& data);
	virtual void Pack(AENotation& data);

private:

	unsigned flags;

};

#endif
